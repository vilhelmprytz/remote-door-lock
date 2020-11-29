import React, { useEffect, useState, useCallback } from "react";
import ApiCall from "../ApiCall";

import Button from "../components/Button";
import Paragraph from "../components/Paragraph";
import Title from "../components/Title";
import Image from "../components/Image";
import Alert from "../components/Alert";

const BACKEND_URL = process.env.REACT_APP_BACKEND_URL;

function Dashboard(props) {
  const [locked, setLocked] = useState(null);
  const [lastUpdated, setLastUpdated] = useState(null);
  const [lastChangedBy, setLastChangedBy] = useState(null);
  const [history, setHistory] = useState([]);
  const [users, setUsers] = useState([]);
  const [newUser, setNewUser] = useState("");
  const [error, setError] = useState("");

  const handleError = (json) => {
    setError({
      code: json.code,
      description: json.description,
      name: json.name,
    });
    throw Error(`${json.description} (${json.code} ${json.name})`);
  };

  const updateInfo = useCallback(() => {
    ApiCall("/api/lock").then((json) => {
      if (json.code === 200) {
        setLocked(json.response.locked);
        setLastUpdated(json.response.last_updated);
        setLastChangedBy(json.response.changed_by);
      } else {
        handleError(json);
      }
    });
    ApiCall("/api/history").then((json) => {
      if (json.code === 200) {
        setHistory(json.response.log);
      } else {
        handleError(json);
      }
    });
    ApiCall("/api/user").then((json) => {
      if (json.code === 200) {
        setUsers(json.response.users);
      } else {
        handleError(json);
      }
    });
  }, []);

  useEffect(updateInfo, [updateInfo]);

  // this is bad practice but works for the scale of this project
  useEffect(() => {
    const interval = setInterval(() => {
      updateInfo();
    }, 1500);
    return () => clearInterval(interval);
  }, [updateInfo]);

  const toggleLock = () => {
    ApiCall("/api/lock", "POST", JSON.stringify({ locked: !locked })).then(
      (json) => {
        if (json.code === 200) {
          setLocked(!locked);
          updateInfo();
        } else {
          handleError(json);
        }
      }
    );
  };

  const deleteUser = (email) => {
    ApiCall("/api/user", "DELETE", JSON.stringify({ email: email })).then(
      (json) => {
        if (json.code === 200) {
          updateInfo();
        } else {
          handleError(json);
        }
      }
    );
  };

  const submitNewUser = () => {
    ApiCall("/api/user", "POST", JSON.stringify({ email: newUser })).then(
      (json) => {
        if (json.code === 200) {
          updateInfo();
          setNewUser("");
        } else {
          handleError(json);
        }
      }
    );
  };

  return (
    <div>
      <Title>Remote Door Lock</Title>
      <Image alt={`Avatar for ${props.user.name}`} src={props.user.avatar} />

      {error && (
        <Alert>
          <b>
            <Paragraph>
              {error.code} {error.name}
            </Paragraph>
          </b>
          <Paragraph>{error.description}</Paragraph>
        </Alert>
      )}

      <Paragraph>
        Signed in as {props.user.name} ({props.user.email})
      </Paragraph>
      <Paragraph>
        The lock is currently <b>{locked ? "locked" : "unlocked"}</b>. Last
        changed at <b>{lastUpdated}</b> by <b>{lastChangedBy}</b>
      </Paragraph>
      <Button primary onClick={() => toggleLock()}>
        {locked ? "Unlock" : "Lock"}
      </Button>
      <a href={`${BACKEND_URL}/api/auth/logout`}>
        <Button>Log out</Button>
      </a>

      <Title>Last 20 events</Title>
      <table>
        <tbody>
          <tr>
            <th>Time</th>
            <th>Action</th>
            <th>User</th>
          </tr>
          {history.map((log) => (
            <tr key={log.id}>
              <td>{log.time_created}</td>
              <td>{log.toggle ? "locked" : "unlocked"}</td>
              <td>{log.email}</td>
            </tr>
          ))}
        </tbody>
      </table>

      <Title>Users</Title>
      <Paragraph>Create new user.</Paragraph>
      <form
        onSubmit={(e) => {
          e.preventDefault();
          submitNewUser();
        }}
      >
        <input
          type="email"
          name="email"
          value={newUser}
          onChange={(e) => setNewUser(e.target.value)}
        ></input>
      </form>
      <table>
        <tbody>
          <tr>
            <th>Email</th>
            <th>Created</th>
            <th>Updated</th>
            <th>Action</th>
          </tr>
          {users.map((user) => (
            <tr key={user.id}>
              <td>{user.email}</td>
              <td>{user.time_created}</td>
              <td>{user.time_updated}</td>
              <td>
                <Button onClick={() => deleteUser(user.email)}>Delete</Button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

export default Dashboard;
