const { app, BrowserWindow } = require('electron')
const path = require('path');
const { FacefullBridgeEventSend, FacefullBridgeEventAttach } = require('./facefull_bridge.js');

let mainWindow = null;

const createWindow = () => {
    mainWindow = new BrowserWindow({
        width: 800,
        height: 700,
        frame: false,
        show: false,
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true,
            preload: path.join(__dirname, 'preload.js')
        }
    });

    mainWindow.loadFile('ui/index.html')
    mainWindow.hide();

    setTimeout(function() {
        mainWindow.show();
    }, 500);
}

app.whenReady().then(() => {
    createWindow();

    FacefullBridgeEventSend(mainWindow,"onWindowCreated", "test");
})

FacefullBridgeEventAttach("doWindowMinimize", function(sender, data) {
    sender.minimize();
});

FacefullBridgeEventAttach("doWindowMaximize", function(sender, data) {
    sender.isMaximized() ? sender.unmaximize() : sender.maximize();
});

FacefullBridgeEventAttach("doWindowClose", function(sender, data) {
    sender.hide();
});
