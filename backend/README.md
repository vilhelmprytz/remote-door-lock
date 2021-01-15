# remote-door-lock/backend

Backend API written in Python (Flask).

## Deployment

This guide will show you how to deploy the API application.

### Deployment Requirements

For deployment, you only need [Docker](https://docs.docker.com/engine/install/ubuntu/) and [docker-compose](https://docs.docker.com/compose).

### Deployment Instructions

Choose whether to use SQLite or MySQL as your database driver. SQLite is great for smaller applications where no traffic is expected or if you are tight on RAM (SQLite is just a `.db` file). MySQL scales a lot better but uses more RAM. You can also easily modify the project to work with PostgreSQL since the module [Flask-SQLAlchemy](https://flask-sqlalchemy.palletsprojects.com), that is used in this project, supports it as well.

#### MySQL

Clone the project to your server. Go to the `backend` folder. Copy the file `.env.mysql.example` to `.env` and modify the variables to suit your setup. All the variables are described in the file.

Once configured, start the app with `docker-compose`.

```bash
docker-compose -f docker-compose.prod.mysql.yml up -d
```

#### SQLite

Clone the project to your server. Go to the `backend` folder. Copy the file `.env.sqlite.example` to `.env` and modify the variables to suit your setup. All the variables are described in the file.

Once configured, start the app with `docker-compose`.

```bash
docker-compose -f docker-compose.prod.mysql.yml up -d
```

### Creating the first user

Once you got the app running, you need to create your first user from the command-line. This can easily be done using docker-compose (replace `mysql` with `sqlite` if you are using SQLite).

```bash
docker-compose -f docker-compose.prod.mysql.yml exec app python scripts/create_user.py
```

### Serving the API

Serving the app on port 80/443 is not suitable. You should use Nginx to `proxy_pass` all requests on `/api` to the Docker container. Then you can use Nginx to serve the static frontend which improves application performance.

Example Nginx location block (this is not a full Nginx configuration file!).

```nginx
location /api {
    # replace 5000 with the port you are using
    proxy_pass http://127.0.0.1:5000;
    proxy_redirect off;
}
```

## Local development

For local development, you do not only need the deployment dependencies but also the following:

- Python 3.9
- [Pipenv](https://github.com/pypa/pipenv)

For testing, you need a MySQL database running locally. We use docker-compose to make this very easy.

```bash
docker-compose up -d
```

Then use Pipenv to make sure you got all the dependencies for the Python application.

```bash
pipenv --python 3.9 install --dev
```

To enter the virtual environment, use Pipenv.

```bash
pipenv shell
```

You can now start the app locally.

```bash
python app.py
```
