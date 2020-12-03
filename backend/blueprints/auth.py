from flask import Blueprint, request, abort, session, redirect

from models import APIResponse
from database_models import User
from decorators.auth import authenticated
from logger import get_logger

from os import environ
from oauthlib.oauth2 import WebApplicationClient
from requests import get, post
from json import dumps

# define configuration (from environment variables)
GOOGLE_CLIENT_ID = environ.get("GOOGLE_CLIENT_ID", None)
GOOGLE_CLIENT_SECRET = environ.get("GOOGLE_CLIENT_SECRET", None)
GOOGLE_DISCOVERY_URL = "https://accounts.google.com/.well-known/openid-configuration"
FRONTEND_URL = environ.get("FRONTEND_URL", None)
BACKEND_URL = environ.get("BACKEND_URL", None)

# OAuth 2 client setup
client = WebApplicationClient(GOOGLE_CLIENT_ID)

# logger
logger = get_logger()

auth_blueprint = Blueprint("auth", __name__, template_folder="../templates")


# functions
def get_google_provider_cfg():
    return get(GOOGLE_DISCOVERY_URL).json()


@auth_blueprint.route("/login")
def login():
    # Find out what URL to hit for Google login
    google_provider_cfg = get_google_provider_cfg()
    authorization_endpoint = google_provider_cfg["authorization_endpoint"]

    # Use library to construct the request for Google login and provide
    # scopes that let you retrieve user's profile from Google
    request_uri = client.prepare_request_uri(
        authorization_endpoint,
        redirect_uri=BACKEND_URL + "/api/auth/login/callback",
        scope=["openid", "email", "profile"],
    )

    return APIResponse(response={"login_url": request_uri}).serialize()


@auth_blueprint.route("/login/callback")
def callback():
    # Get authorization code Google sent back
    code = request.args.get("code")

    # If no code was sent
    if not code:
        abort(400, "missing oauth token")

    # Find out what URL to hit to get tokens that allow you to ask for
    # things on behalf of a user
    google_provider_cfg = get_google_provider_cfg()
    token_endpoint = google_provider_cfg["token_endpoint"]

    # Prepare and send a request to get tokens
    token_url, headers, body = client.prepare_token_request(
        token_endpoint,
        authorization_response=request.url,
        redirect_url=BACKEND_URL + "/api/auth/login/callback",
        code=code,
    )
    token_response = post(
        token_url,
        headers=headers,
        data=body,
        auth=(GOOGLE_CLIENT_ID, GOOGLE_CLIENT_SECRET),
    )

    # Parse the tokens
    client.parse_request_body_response(dumps(token_response.json()))

    # Now that you have tokens let's find and hit the URL
    # from Google that gives you the user's profile information,
    # including their Google profile image and email
    userinfo_endpoint = google_provider_cfg["userinfo_endpoint"]
    uri, headers, body = client.add_token(userinfo_endpoint)
    userinfo_response = get(uri, headers=headers, data=body)

    # You want to make sure their email is verified.
    # The user authenticated with Google, authorized your
    # app, and now you've verified their email through Google!
    if userinfo_response.json().get("email_verified"):
        user = User.query.filter_by(email=userinfo_response.json()["email"]).all()

        if not user:
            logger.info(
                f"User {userinfo_response.json()['email']} tried to sign-in, not allowed"
            )
            abort(401, "Google Account not allowed to sign-in here")

        session["authenticated"] = True
        session["google_unique_id"] = userinfo_response.json()["sub"]
        session["google_email"] = userinfo_response.json()["email"]
        session["google_picture_url"] = userinfo_response.json()["picture"]
        session["google_name"] = userinfo_response.json()["name"]

        return redirect(FRONTEND_URL)

    abort(400, "User email not available or not verified by Google")


@auth_blueprint.route("/validate")
@authenticated
def validate():
    return APIResponse(
        response={
            "authenticated": session["authenticated"],
            "email": session["google_email"],
            "name": session["google_name"],
            "avatar": session["google_picture_url"],
        }
    ).serialize()


@auth_blueprint.route("/logout")
@authenticated
def logout():
    session.pop("authenticated", None)

    return redirect(FRONTEND_URL)
