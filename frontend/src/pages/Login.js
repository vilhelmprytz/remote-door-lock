import React, { useEffect, useState } from "react";
import ApiCall from "./../ApiCall";

import Button from "../components/Button";
import Title from "../components/Title";

function Login() {
  const [googleUrl, setGoogleUrl] = useState("");

  useEffect(() => {
    ApiCall("/api/auth/login").then((json) => {
      if (json.code === 200) {
        setGoogleUrl(json.response.login_url);
      } else {
        throw Error(`${json.description} (${json.code} ${json.name})`);
      }
    });
  }, []);

  return (
    <div>
      <Title>Remote Door Lock</Title>
      <a href={googleUrl}>
        <Button primary>Sign-in with Google</Button>
      </a>
    </div>
  );
}

export default Login;
