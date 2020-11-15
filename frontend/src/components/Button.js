import styled from "styled-components";

const Button = styled.button`
  background: ${(props) => (props.primary ? "#d21919" : "white")};
  color: ${(props) => (props.primary ? "white" : "#d21919")};
  font-size: 1em;
  margin: 1em;
  padding: 0.25em 1em;
  border: 2px solid #d21919;
  border-radius: 3px;
`;

export default Button;
