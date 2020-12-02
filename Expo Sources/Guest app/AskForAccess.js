import { Button } from 'native-base';
import React from 'react';
import { View, Text, Platform } from 'react-native';
import axios from 'axios';

class AskForAccess extends React.Component {

    sendRequest = async () => {
        try {
            await axios.post("https://api.particle.io/v1/devices/e00fce68d852f1805a0f3924/app_request?access_token=c5b8d2c37998f929549d12997306426221ce5a31", "arg={test:test}")
            console.log("Sending Request...")
        }
        catch(error) {
            console.log(error)
        }
    }

    render() {
        return (
            <View style={styles.container}>
                <Text style={styles.header}> Press a name to ask for access! </Text>
                <Button info style={styles.buttonStyle} onPress={() => this.sendRequest()}>
                    <Text style={styles.buttonText}>  Anders Andersen  </Text>
                </Button>
            </View>
        );
    }
}

const styles = {
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
        marginTop: 20,
        margin: 10,
        alignItems: 'center',
        alignSelf: 'center',
    },
    buttonText : {
        color: 'white',
    },
}

export default AskForAccess;