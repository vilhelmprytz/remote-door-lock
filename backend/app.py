from flask import Flask, json, jsonify, request, Response, abort
from werkzeug.exceptions import HTTPException


class ApiResponse:
    def __init__(self, code=200, name="OK", description="success", response={}):
        self.code = code
        self.name = name
        self.description = description
        self.response = response


# base response
class ApplicationResponse(Response):
    default_mimetype = "application/json"


# temporary locked status
class Lock:
    def __init__(self):
        self.locked = False


app = Flask(__name__)
app.response_class = ApplicationResponse

door_lock = Lock()

# all error pages are now JSON instead of HTML
@app.errorhandler(HTTPException)
def handle_exception(e):
    response = e.get_response()

    response.data = json.dumps(
        vars(
            ApiResponse(
                code=e.code, name=e.name, description=e.description, response={}
            )
        )
    )

    response.content_type = "application/json"
    return response, e.code


@app.route("/api/lock", methods=["GET", "POST"])
def lock():
    if request.method == "POST":
        content = request.json

        if not "locked" in content:
            abort(400, "missing variable locked in request json")

        door_lock.locked = content["locked"]

    return jsonify(vars(ApiResponse(response={"locked": door_lock.locked})))


if __name__ == "__main__":
    app.run(host="0.0.0.0")
