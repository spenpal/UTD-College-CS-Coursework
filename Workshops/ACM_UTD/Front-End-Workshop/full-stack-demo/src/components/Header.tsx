import React from 'react';
 
/** Types in Typescript:
 * string
 * number
 * object
 * null
 * undefined
 * Arrays
 */

type Props = {
    text?: string,      // '?' implies optional parameter
    color?: string
}

const defaultProps: Props = {
    text: 'default text', 
    color: 'white'
}

const fn = (color: string = 'white') => {
    console.log(color);
}


const Header = (props: Props) => {
    console.log(props.text);

    return (<div style={{ backgroundColor: (props.color === undefined) ? defaultProps.color : props.color }}>
        { (props.text === undefined) ? defaultProps.text : props.text }
    </div>)
}

export default Header;