# remote-door-lock/backend

Python backend.

## Requirements

For deployment, you only need Docker and docker-compose.

For local development, you also need the following:

- Python 3.9
- [Pipenv](https://github.com/pypa/pipenv)

## Environment Variables

Environment variables required for the backend to run. Define in `.env` file. As a reference, you may copy the `.env.example` and modify it to suit your setup.

### Required Environment Variables

- `GOOGLE_CLIENT_ID` - Google Client ID
- `GOOGLE_CLIENT_SECRET` - Google Client Secret
- `FRONTEND_URL` - The URL where the frontend is served.
- `BACKEND_URL` - Base URL where backend API is running. The value for this can be same as for `FRONTEND_URL`.
- `PORT` - Required for deployment. Port the production deployment will listen to.

### Optional Environment Variables

- `MYSQL_USER` - MySQL username, overrides default `remote-door-lock`
- `MYSQL_PASSWORD` - MySQL password, overrides default `password`
- `MYSQL_HOST` - MySQL host, overrides default `127.0.0.1`. Use `host.machine` and the app will use the internal IP of the host machine (e.g. any MySQL instance running on the same Docker host but outside the container).
- `MYSQL_DATABASE` - MySQL database, overrides default `remote-door-lock`
- `REDIS_HOST` - Hostname for Redis server, overrides default `localhost`

### Development Environment Variables

- `OAUTHLIB_INSECURE_TRANSPORT` - OAuthLib does not run with HTTP. If you are (dangerous!), use this to override.
- `INSECURE` - Enforces insecure CORS and cookie settings. Only for local development.

## Deployment Instructions

Create a `.env` file according to [the instructions above](#environment-variables).

Use `docker-compose` to start the container.

```bash
docker-compose -f prod.yml up -d
```

The API will then start, listening to port 7000. You can then use Nginx to `proxy_pass` all requests on `/api` to the Docker container.

Example Nginx location block.

```nginx
location /api {
    proxy_pass http://127.0.0.1:7000;
}
```
