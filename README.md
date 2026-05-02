# Fireaim - 3D Sci-Fi Shooting Game

A 3D first-person shooting game built with React Native and native Android OpenGL.

## Features
- First-person 3D shooting with native OpenGL ES
- Touch controls - drag to aim, tap to shoot
- Wave-based gameplay with increasing difficulty
- Combo scoring system
- Neon sci-fi visual style

## Tech Stack
- **Frontend**: React Native 0.74
- **3D Rendering**: Native Android OpenGL ES
- **Language**: Java + JavaScript
- **Build**: Gradle (Android)

## Controls
- **Drag** - Aim around
- **Tap** - Shoot
- **Destroy** all targets to advance wave

## Local Development

```bash
# Install dependencies
npm install

# Run on Android
npm run android

# Build APK
cd android && ./gradlew assembleRelease
```

## GitHub Actions
The project automatically builds Android APKs on every push via GitHub Actions.

## License
MIT