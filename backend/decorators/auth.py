from functools import wraps
from flask import session, abort

from database_models import User


def authenticated(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if not session.get("authenticated", None):
            abort(401, "not authenticated")

        if not User.query.filter_by(email=session["google_email"]).all():
            abort(401, "Google Account not allowed to sign in")

        return f(*args, **kwargs)

    return decorated_function
