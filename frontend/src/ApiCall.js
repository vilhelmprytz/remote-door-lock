const BACKEND_URL = process.env.REACT_APP_BACKEND_URL;
const ALLOW_CROSS_COOKIE = process.env.REACT_APP_ALLOW_CROSS_COOKIE;

function ApiCall(url, method = "GET", json_body = null) {
  return fetch(`${BACKEND_URL}${url}`, {
    credentials: ALLOW_CROSS_COOKIE ? "include" : "same-origin",
    method: method,
    headers: {
      Accept: "application/json",
      "Content-Type": "application/json",
    },
    body: json_body,
  })
    .then((response) => {
      const contentType = response.headers.get("content-type");
      if (contentType && contentType.indexOf("application/json") !== 1) {
        return response.json();
      } else {
        throw new Error(
          `API response not valid JSON on /${url} (${response.status} ${response.statusText})`
        );
      }
    })
    .then((response) => {
      return response;
    });
}

export default ApiCall;
