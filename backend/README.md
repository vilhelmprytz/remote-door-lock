# remote-door-lock/backend

Python backend.

## Environment Variables

Environment variables required for the backend to run. Define in `.env` file. As a reference, you may copy the `.env.example` and modify it to suit your setup.

### Required Environment Variables

* `MYSQL_USER` - MySQL username, overrides default `remote-door-lock`
* `MYSQL_PASSWORD` - MySQL password, overrides default `password`
* `MYSQL_HOST` - MySQL host, overrides default `127.0.0.1`
* `MYSQL_DATABASE` - MySQL database, overrides default `remote-door-lock`
* `GOOGLE_CLIENT_ID` - Google Client ID
* `GOOGLE_CLIENT_SECRET` - Google Client Secret
* `FRONTEND_URL` - The URL where the frontend is served.
* `BACKEND_URL` - Base URL where backend API is running. The value for this can be same as for `FRONTEND_URL`.

### Optional Environment Variables

* `OAUTHLIB_INSECURE_TRANSPORT` - OAuthLib does not run with HTTP. If you are (dangerous!), use this to override.
* `INSECURE` - Enforces insecure CORS and cookie settings. Only for local development.
