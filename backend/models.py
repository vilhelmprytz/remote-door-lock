from flask import jsonify


class APIResponse:
    def __init__(self, code=200, name="OK", description="success", response={}):
        self.code = code
        self.name = name
        self.description = description
        self.response = response

    def serialize(self):
        return jsonify(self.__dict__), self.code
