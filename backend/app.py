from flask import Flask, json, request, abort, session
from werkzeug.exceptions import HTTPException
from flask_session import Session
from flask_cors import CORS
from redis import Redis
from datetime import timedelta
from os import environ
from sqlalchemy import desc

from models import APIResponse
from database_models import db, Lock, User
from blueprints.auth import auth_blueprint
from decorators.auth import authenticated

app = Flask(__name__)

INSECURE = environ.get("INSECURE", False)
FRONTEND_URL = environ.get("FRONTEND_URL", None)

MYSQL_USER = environ.get("MYSQL_USER", "remote_door_lock")
MYSQL_PASSWORD = environ.get("MYSQL_PASSWORD", "password")
MYSQL_HOST = environ.get("MYSQL_HOST", "127.0.0.1")
MYSQL_DATABASE = environ.get("MYSQL_DATABASE", "remote_door_lock")

app.config["SESSION_TYPE"] = "redis"
app.config["SESSION_REDIS"] = Redis(host=environ.get("REDIS_HOST", "localhost"), db=0)
app.config["SESSION_COOKIE_SAMESITE"] = "Strict"
app.config["SESSION_PERMANENT"] = True
app.config["PERMANENT_SESSION_LIFETIME"] = timedelta(hours=24)

app.config[
    "SQLALCHEMY_DATABASE_URI"
] = f"mysql+pymysql://{MYSQL_USER}:{MYSQL_PASSWORD}@{MYSQL_HOST}/{MYSQL_DATABASE}?charset=utf8mb4"
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False

if INSECURE:
    CORS(app, supports_credentials=True)
else:
    app.config["SESSION_COOKIE_SECURE"] = True
    CORS(app, resources={r"/*": {"origins": FRONTEND_URL}})

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
    @authenticated
    def _post():
        content = request.json

        if "locked" not in content:
            abort(400, "missing variable locked in request json")

        update = Lock(email=session["google_email"], toggle=content["locked"])

        db.session.add(update)
        db.session.commit()

    if request.method == "POST":
        _post()

    # determine value
    last = db.session.query(Lock).order_by(Lock.id.desc()).first()
    locked = False if not last else last.toggle
    last_updated = None if not last else str(last.time_created)
    changed_by = None if not last else last.email

    return APIResponse(
        response={
            "locked": locked,
            "last_updated": last_updated,
            "changed_by": changed_by,
        }
    ).serialize()


@app.route("/api/history", methods=["GET"])
@authenticated
def history():
    log = [
        {
            "email": record.email,
            "toggle": record.toggle,
            "time_created": str(record.time_created),
            "time_updated": str(record.time_updated),
        }
        for record in Lock.query.order_by(desc(Lock.id)).limit(20).all()
    ]

    return APIResponse(response={"log": log}).serialize()


@app.route("/api/user", methods=["POST", "GET", "DELETE"])
@authenticated
def user():
    if request.method == "POST":
        content = request.json

        if "email" not in content:
            abort(400, "missing key email")

        user = User(email=content["email"])

        db.session.add(user)
        db.session.commit()

    if request.method == "DELETE":
        content = request.json

        if "email" not in content:
            abort(400, "missing key email")

        user = User.query.filter_by(email=content["email"]).all()

        if len(user) != 1:
            abort(404, "user does not exist")

        user = user[0]

        db.session.delete(user)
        db.session.commit()

    users = [
        {
            "email": record.email,
            "time_created": str(record.time_created),
            "time_updated": str(record.time_updated),
        }
        for record in User.query.all()
    ]

    return APIResponse(response={"users": users}).serialize()


app.register_blueprint(auth_blueprint, url_prefix="/api/auth")

if __name__ == "__main__":
    app.run(host="0.0.0.0")
