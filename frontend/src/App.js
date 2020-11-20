import React, { useEffect, useState } from "react";
import ApiCall from "./ApiCall";

import Login from "./pages/Login";
import Dashboard from "./pages/Dashboard";
import Error from "./pages/Error";

function App() {
  const [page, setPage] = useState("loading");
  const [user, setUser] = useState({});
  const [error, setError] = useState({});

  useEffect(() => {
    ApiCall("/api/auth/validate").then((json) => {
      if (json.code === 200) {
        setUser(json.response);
        setPage("dashboard");
      } else if (json.code === 401) {
        setPage("login");
      } else {
        setPage("error");
        setError({
          code: json.code,
          description: json.description,
          name: json.name,
        });
      }
    });
  }, []);

  if (page === "login") {
    return <Login />;
  } else if (page === "dashboard") {
    return <Dashboard user={user} />;
  } else if (page === "error") {
    return <Error error={error} />;
  } else {
    return <h1>Loading...</h1>;
  }
}

export default App;
