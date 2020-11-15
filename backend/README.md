# remote-door-lock/backend

Python backend.

## Requirements

For deployment, you only need Docker and docker-compose.

For local development, you also need the following:

* Python 3.9
* [Pipenv](https://github.com/pypa/pipenv)

## Environment Variables

Environment variables required for the backend to run. Define in `.env` file. As a reference, you may copy the `.env.example` and modify it to suit your setup.

### Required Environment Variables

* `GOOGLE_CLIENT_ID` - Google Client ID
* `GOOGLE_CLIENT_SECRET` - Google Client Secret
* `FRONTEND_URL` - The URL where the frontend is served.
* `BACKEND_URL` - Base URL where backend API is running. The value for this can be same as for `FRONTEND_URL`.
* `PORT` - Required for deployment. Port the production deployment will listen to.

### Optional Environment Variables

* `MYSQL_USER` - MySQL username, overrides default `remote-door-lock`
* `MYSQL_PASSWORD` - MySQL password, overrides default `password`
* `MYSQL_HOST` - MySQL host, overrides default `127.0.0.1`
* `MYSQL_DATABASE` - MySQL database, overrides default `remote-door-lock`
* `REDIS_HOST` - Hostname for Redis server, overrides default `localhost`

### Development Environment Variables

* `OAUTHLIB_INSECURE_TRANSPORT` - OAuthLib does not run with HTTP. If you are (dangerous!), use this to override.
* `INSECURE` - Enforces insecure CORS and cookie settings. Only for local development.

## Deployment Instructions

Create a `.env` file according to [the instructions above](#environment-variables).

Use `docker-compose` to start the container.

```bash
docker-compose -f prod.yml up -d
```
