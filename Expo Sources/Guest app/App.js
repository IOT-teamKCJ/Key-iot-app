import React from 'react';
import { View, Platform } from 'react-native';
import Landing from './Landing';
import AskForAccess from './AskForAccess';


export default class App extends React.Component {
  state = {
    currentScreen: "landing"
  }

  switchScreen = (currentScreen) => {
    this.setState({currentScreen});
  }

  renderScreen = () => {
    if (this.state.currentScreen == "landing") {
      return (
        <Landing switchScreen={this.switchScreen}/>
      )
    } 
    else if (this.state.currentScreen == "AskForAccess") {
      return (
        <AskForAccess />
      )
    }
  }

  render() {
    return (
      <View style={styles.container}>
        {this.renderScreen()}
      </View>
    );
  }
}

const styles = {
  container: {
    flex: 1,
    marginTop: Platform.OS == "android" ? 24 : 0,
  },
}