import React, { Component } from 'react';
import {
  View,
  Text,
  TouchableOpacity,
  StyleSheet,
  requireNativeComponent,
  NativeModules,
  Platform
} from 'react-native';

const GameGLSurfaceView = Platform.OS === 'android' 
  ? requireNativeComponent('GameGLSurfaceView')
  : View;

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#0a0a1a',
  },
  menuContainer: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  title: {
    fontSize: 56,
    fontWeight: 'bold',
    color: '#00ff88',
    letterSpacing: 8,
    marginBottom: 10,
  },
  subtitle: {
    fontSize: 18,
    color: '#00ffff',
    letterSpacing: 12,
    marginBottom: 40,
  },
  highScoreBox: {
    borderWidth: 2,
    borderColor: '#ff0080',
    padding: 20,
    borderRadius: 8,
    marginBottom: 40,
    alignItems: 'center',
  },
  highScoreLabel: {
    color: '#ff0080',
    fontSize: 14,
    fontWeight: 'bold',
  },
  highScoreValue: {
    color: '#fff',
    fontSize: 28,
    fontWeight: 'bold',
    marginTop: 5,
  },
  startButton: {
    backgroundColor: '#00ff88',
    paddingHorizontal: 50,
    paddingVertical: 16,
    borderRadius: 30,
  },
  startText: {
    color: '#0a0a1a',
    fontSize: 24,
    fontWeight: 'bold',
    letterSpacing: 4,
  },
  instructions: {
    color: '#666',
    fontSize: 14,
    marginTop: 30,
    textAlign: 'center',
  },
  hud: {
    position: 'absolute',
    top: 40,
    left: 20,
    right: 20,
    flexDirection: 'row',
    justifyContent: 'space-between',
  },
  scoreBox: {
    backgroundColor: 'rgba(0,0,0,0.5)',
    padding: 12,
    borderRadius: 8,
    borderWidth: 2,
    borderColor: '#00ff88',
  },
  waveBox: {
    backgroundColor: 'rgba(0,0,0,0.5)',
    padding: 12,
    borderRadius: 8,
    borderWidth: 2,
    borderColor: '#00ffff',
  },
  label: {
    fontSize: 12,
    fontWeight: 'bold',
  },
  scoreLabel: { color: '#00ff88' },
  waveLabel: { color: '#00ffff' },
  value: {
    fontSize: 24,
    fontWeight: 'bold',
    color: '#fff',
  },
  comboBox: {
    position: 'absolute',
    top: 120,
    alignSelf: 'center',
    backgroundColor: 'rgba(255,0,128,0.8)',
    paddingHorizontal: 16,
    paddingVertical: 6,
    borderRadius: 20,
  },
  comboText: {
    color: '#fff',
    fontSize: 18,
    fontWeight: 'bold',
  },
  gameOverOverlay: {
    ...StyleSheet.absoluteFillObject,
    backgroundColor: 'rgba(10,10,26,0.9)',
    justifyContent: 'center',
    alignItems: 'center',
  },
  gameOverTitle: {
    fontSize: 48,
    fontWeight: 'bold',
    color: '#ff0080',
    marginBottom: 30,
  },
  finalScoreBox: {
    borderWidth: 2,
    borderColor: '#00ff88',
    padding: 30,
    borderRadius: 16,
    alignItems: 'center',
    marginBottom: 40,
  },
  finalScoreLabel: {
    color: '#00ffff',
    fontSize: 16,
    letterSpacing: 4,
  },
  finalScoreValue: {
    color: '#fff',
    fontSize: 48,
    fontWeight: 'bold',
    marginTop: 10,
  },
  restartButton: {
    backgroundColor: '#00ff88',
    paddingHorizontal: 40,
    paddingVertical: 14,
    borderRadius: 25,
    marginBottom: 15,
  },
  restartText: {
    color: '#0a0a1a',
    fontSize: 18,
    fontWeight: 'bold',
  },
  menuButton: {
    borderWidth: 2,
    borderColor: '#666',
    paddingHorizontal: 40,
    paddingVertical: 14,
    borderRadius: 25,
  },
  menuText: {
    color: '#666',
    fontSize: 18,
    fontWeight: 'bold',
  },
});

class App extends Component {
  state = {
    gameState: 'menu',
    score: 0,
    wave: 1,
    combo: 0,
    highScore: 0,
  };

  startGame = () => {
    this.setState({
      gameState: 'playing',
      score: 0,
      wave: 1,
      combo: 0,
    });
  };

  gameOver = () => {
    const { score, highScore } = this.state;
    if (score > highScore) {
      this.setState({ highScore: score });
    }
    this.setState({ gameState: 'gameover' });
  };

  returnToMenu = () => {
    this.setState({ gameState: 'menu' });
  };

  renderMenu() {
    return (
      <View style={styles.menuContainer}>
        <Text style={styles.title}>FIREAIM</Text>
        <Text style={styles.subtitle}>3D SHOOTER</Text>
        
        <View style={styles.highScoreBox}>
          <Text style={styles.highScoreLabel}>HIGH SCORE</Text>
          <Text style={styles.highScoreValue}>{this.state.highScore}</Text>
        </View>
        
        <TouchableOpacity style={styles.startButton} onPress={this.startGame}>
          <Text style={styles.startText}>START</Text>
        </TouchableOpacity>
        
        <Text style={styles.instructions}>
          Drag to aim • Tap to shoot{'\n'}
          Destroy all targets to advance
        </Text>
      </View>
    );
  }

  renderGame() {
    return (
      <View style={styles.container}>
        <GameGLSurfaceView 
          style={{flex: 1}}
          gameState={this.state.gameState}
        />
        
        <View style={styles.hud}>
          <View style={styles.scoreBox}>
            <Text style={[styles.label, styles.scoreLabel]}>SCORE</Text>
            <Text style={styles.value}>{this.state.score}</Text>
          </View>
          <View style={styles.waveBox}>
            <Text style={[styles.label, styles.waveLabel]}>WAVE</Text>
            <Text style={styles.value}>{this.state.wave}</Text>
          </View>
        </View>
        
        {this.state.combo > 1 && (
          <View style={styles.comboBox}>
            <Text style={styles.comboText}>{this.state.combo}x COMBO!</Text>
          </View>
        )}
      </View>
    );
  }

  renderGameOver() {
    return (
      <View style={styles.container}>
        <GameGLSurfaceView style={{flex: 1}} gameState="gameover" />
        
        <View style={styles.gameOverOverlay}>
          <Text style={styles.gameOverTitle}>GAME OVER</Text>
          
          <View style={styles.finalScoreBox}>
            <Text style={styles.finalScoreLabel}>FINAL SCORE</Text>
            <Text style={styles.finalScoreValue}>{this.state.score}</Text>
          </View>
          
          <TouchableOpacity style={styles.restartButton} onPress={this.startGame}>
            <Text style={styles.restartText}>PLAY AGAIN</Text>
          </TouchableOpacity>
          
          <TouchableOpacity style={styles.menuButton} onPress={this.returnToMenu}>
            <Text style={styles.menuText}>MAIN MENU</Text>
          </TouchableOpacity>
        </View>
      </View>
    );
  }

  render() {
    const { gameState } = this.state;
    
    if (gameState === 'menu') return this.renderMenu();
    if (gameState === 'gameover') return this.renderGameOver();
    return this.renderGame();
  }
}

export default App;