import Constants from 'expo-constants';
import * as Notifications from 'expo-notifications';
import * as Permissions from 'expo-permissions';
import React, { useState, useEffect, useRef } from 'react';
import { Text, View, Platform, Button } from 'react-native';
import axios from 'axios';


Notifications.setNotificationHandler({
  handleNotification: async () => ({
    shouldShowAlert: true,
    shouldPlaySound: false,
    shouldSetBadge: false,
  }),
});

export default function App() {
  const [expoPushToken, setExpoPushToken] = useState('');
  const [notification, setNotification] = useState(false);
  const notificationListener = useRef();
  const responseListener = useRef();

  useEffect(() => {
    registerForPushNotificationsAsync().then(token => setExpoPushToken(token));

    // This listener is fired whenever a notification is received while the app is foregrounded
    notificationListener.current = Notifications.addNotificationReceivedListener(notification => {
      setNotification(notification);
    });

    // This listener is fired whenever a user taps on or interacts with a notification (works when app is foregrounded, backgrounded, or killed)
    responseListener.current = Notifications.addNotificationResponseReceivedListener(response => {
      console.log(response);
    });

    return () => {
      Notifications.removeNotificationSubscription(notificationListener);
      Notifications.removeNotificationSubscription(responseListener);
    };
  }, []);

  return (
    <View style={styles.viewStyle}>
        <Text>Your expo push token: {expoPushToken}</Text>
        <Button color='green' style={styles.buttonStyleAccept} title={"Accept"} onPress={() => acceptCurl()}></Button>
        <Button color='red' style={styles.buttonStyleDecline} title={"Decline"} onPress={() => declineCurl()}></Button>
    </View>
  );
}

async function acceptCurl()  {
  try {
    await axios.post("https://api.particle.io/v1/devices/e00fce68d852f1805a0f3924/master_command?access_token=c5b8d2c37998f929549d12997306426221ce5a31", "arg={access:Accept}")
    console.log("Accepting...")
    }
  catch (error) {
    console.log(error)
  }
}

async function declineCurl() {
  try {
    await axios.post("https://api.particle.io/v1/devices/e00fce68d852f1805a0f3924/master_command?access_token=c5b8d2c37998f929549d12997306426221ce5a31", "arg={access:Deny}")
    console.log("Declining...")
    }
  catch (error) {
    console.log(error)
  }
}

// Can use this function below, OR use Expo's Push Notification Tool-> https://expo.io/notifications
async function sendPushNotification(expoPushToken) {
  const message = {
    to: expoPushToken,
    sound: 'default',
    title: 'Original Title',
    body: 'And here is the body!',
    data: { data: 'goes here' },
  };

  await fetch('https://exp.host/--/api/v2/push/send', {
    method: 'POST',
    headers: {
      Accept: 'application/json',
      'Accept-encoding': 'gzip, deflate',
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(message),
  });
}

async function registerForPushNotificationsAsync() {
  let token;
  if (Constants.isDevice) {
    const { status: existingStatus } = await Permissions.getAsync(Permissions.NOTIFICATIONS);
    let finalStatus = existingStatus;
    if (existingStatus !== 'granted') {
      const { status } = await Permissions.askAsync(Permissions.NOTIFICATIONS);
      finalStatus = status;
    }
    if (finalStatus !== 'granted') {
      alert('Failed to get push token for push notification!');
      return;
    }
    token = (await Notifications.getExpoPushTokenAsync()).data;
    console.log(token);
  } else {
    alert('Must use physical device for Push Notifications');
  }

  if (Platform.OS === 'android') {
    Notifications.setNotificationChannelAsync('default', {
      name: 'default',
      importance: Notifications.AndroidImportance.MAX,
      vibrationPattern: [0, 250, 250, 250],
      lightColor: '#FF231F7C',
    });
  }

  return token;
}
const styles = {
  viewStyle: {
    marginTop: Platform.OS == 'android' ? 24 : 0,
    flex: 1,
    flexDirection: 'column',
    justifyContent: 'center',
    alignItems: 'center',
  },
  buttonStyleAccept: {
    marginTop: 100,
    margin: 10,
    alignItems: 'center',
    alignSelf: 'center',
    justifyContent: 'center'
  },
  buttonStyleDecline: {
    marginTop: 100,
    margin: 10,
    alignItems: 'center',
    alignSelf: 'center',
    justifyContent: 'center'
  },
}