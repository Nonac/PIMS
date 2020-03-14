#PIMS Destop App
A minimal Electron application running a p5.js sketch.
```bash
# Install Eletron
npm install --save-dev electron
# Clone this repository
git clone https://github.com/Nonac/PIMS/
# Go into the repository
cd PIMS
git checkout dev
cd destop_app
# Install dependencies
npm install
# Run the app
npm start
```
Jetbrains Webstorm is recommended as IDE.
```$xslt
Run -> Edit Configurations...

# find Eletron app in desktop_app/node_modules/...
# the Eletron app exits iff npm installed
Node Interpreter: 
Working dictionary: ~/PIMS
JavaScript file: desktop_app/main.js
```
