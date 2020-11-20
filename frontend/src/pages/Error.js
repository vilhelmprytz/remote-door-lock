import React from "react";

import Title from "../components/Title";
import Alert from "../components/Alert";
import Paragraph from "../components/Paragraph";

function Error(props) {
  return (
    <div>
      <Title>Remote Door Lock</Title>
      <Alert>
        <b>
          <Paragraph>
            {props.error.code} {props.error.name}
          </Paragraph>
        </b>
        <Paragraph>{props.error.description}</Paragraph>
      </Alert>
    </div>
  );
}

export default Error;
