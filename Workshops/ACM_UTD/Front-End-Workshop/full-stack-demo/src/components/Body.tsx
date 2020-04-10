import React from 'react';
import axios from 'axios';

type Props = {

}

type State = {
    username: string,
    password: string
}

class Body extends React.Component<Props, State> {
    state : State = {
        username: '',
        password: ''
    }

    handleUsername = (e: React.FormEvent<HTMLInputElement>) => {
        // Do Username Setting State Here
        const newUsername: string = e.currentTarget.value;

        this.setState(function setState() {
            return { username: newUsername } 
        })
    }

    handlePassword = (e: React.FormEvent<HTMLInputElement>) => {
        // Do Password Setting State Here
        const newPassword: string = e.currentTarget.value;

        this.setState(() => {
            return { password: newPassword }
        })
    }

    handleLoginAttempt = (e) => {
        // Handle Login Attempt Here
        window.alert(`Attemped Login With ${this.state.username}, ${this.state.password}`)
    }

    render() {
        return (<div>
            Body
            <div>
                <label about="Username">Username: </label>
                <input onChange={this.handleUsername} name="username"/>
            </div>
            <div>
                <label about="Password">Password: </label>
                <input onChange={this.handlePassword} name="password" />
            </div>
            <button onClick={this.handleLoginAttempt} type="submit">Log In</button>
        </div>)

    }
}

export default Body;