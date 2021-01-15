FROM python:3.9-slim-buster

WORKDIR /var/www/app

# create .venv dir (this is where pipenv will install)
RUN mkdir .venv

# install dep
RUN pip install --upgrade pip
RUN pip install pipenv
RUN apt-get update && apt-get install iproute2 -y

COPY . /var/www/app

RUN pipenv install --deploy

ENV PATH="/var/www/app/.venv/bin:$PATH"

EXPOSE 5000
CMD [ "/var/www/app/entrypoint.sh" ]
