from flask import Flask, json, request, abort
from werkzeug.exceptions import HTTPException
from flask_session import Session
from redis import Redis
from datetime import timedelta
from os import environ

from models import APIResponse
from database_models import db, Lock, User
from blueprints.auth import auth_blueprint
from decorators.auth import authenticated

app = Flask(__name__)

MYSQL_USER = environ.get("MYSQL_USER", "booking")
MYSQL_PASSWORD = environ.get("MYSQL_PASSWORD", "password")
MYSQL_HOST = environ.get("MYSQL_HOST", "127.0.0.1")
MYSQL_DATABASE = environ.get("MYSQL_DATABASE", "booking")

app.config["SESSION_TYPE"] = "redis"
app.config["SESSION_REDIS"] = Redis(host=environ.get("REDIS_HOST", "localhost"), db=0)
app.config["SESSION_COOKIE_SECURE"] = True
app.config["SESSION_PERMANENT"] = True
app.config["PERMANENT_SESSION_LIFETIME"] = timedelta(hours=24)
app.config[
    "SQLALCHEMY_DATABASE_URI"
] = f"mysql+pymysql://{MYSQL_USER}:{MYSQL_PASSWORD}@{MYSQL_HOST}/{MYSQL_DATABASE}?charset=utf8mb4"
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False

db.init_app(app)

with app.app_context():
    db.create_all()

Session(app)


# all error pages are now JSON instead of HTML
@app.errorhandler(HTTPException)
def handle_exception(e):
    response = e.get_response()

    response.data = json.dumps(
        APIResponse(
            code=e.code, name=e.name, description=e.description, response={}
        ).__dict__
    )

    response.content_type = "application/json"
    return response, e.code


@app.route("/api/lock", methods=["GET", "POST"])
def lock():
    # determine value
    last = db.session.query(Lock).order_by(Lock.id.desc()).first()
    locked = False if not last else last.toggle

    if request.method == "POST":
        content = request.json

        if "locked" not in content:
            abort(400, "missing variable locked in request json")

        print(content["locked"])

    return APIResponse(response={"locked": locked}).serialize()


@app.route("/api/history", methods=["GET"])
@authenticated
def history():
    return APIResponse(response={"log": Lock.query.all()}).serialize()


@app.route("/api/user", methods=["POST", "GET"])
@authenticated
def user():
    if request.method == "POST":
        content = request.json

        if "email" not in content:
            abort(400, "missing key email")

        user = User(email=content["email"])

        db.session.add(user)
        db.session.commit()

    return APIResponse(response={"users": User.query.all()}).serialize()


app.register_blueprint(auth_blueprint, url_prefix="/api/auth")

if __name__ == "__main__":
    app.run(host="0.0.0.0")
