import { Button } from 'native-base';
import React from 'react';
import { View, Image, Text, Platform } from 'react-native';


var landingPage = require('./assets/landing.png')

class Landing extends React.Component {
    render() {
        return (
            <View>
                <View style={styles.viewStyle}>
                    <Image source={landingPage} style={styles.imageStyle}/>
                    <Text style={styles.header}>Welcome to the AccessApp!</Text>
                </View>
                <View style={styles.viewStyle}>
                    <Button info style={styles.buttonStyle} onPress={ () => this.props.switchScreen("AskForAccess")}>
                        <Text style={styles.buttonText}> Let's Get Started </Text>
                    </Button>
                </View>
            </View>
        );
    }
}

const styles = {
    imageStyle: {
        marginTop: 350,
        width: 310,
        height: 270,
        justifyContent: 'center',
    },
    viewStyle: {
        marginTop: Platform.OS == 'android' ? 24 : 0,
        flex: 1,
        flexDirection: 'column',
        justifyContent: 'center',
        alignItems: 'center',
    },
    header: {
        fontSize: 24,
        color: '#1E88E5',
        fontWeight: 'bold',
        alignItems: 'center',
        alignSelf: 'center',
    },
    buttonStyle: {
        marginTop: 730,
        margin: 10,
        alignItems: 'center',
        alignSelf: 'center',
    },
    buttonText : {
        color: 'white',
    },
}

export default Landing;