import sys
from pathlib import Path

# add parent folder
sys.path.append(str(Path(__file__).parent.parent.absolute()))

from app import db, app
from database_models import User

email = input("Enter email: ")

user = User(email=email)

with app.app_context():
    db.session.add(user)
    db.session.commit()
