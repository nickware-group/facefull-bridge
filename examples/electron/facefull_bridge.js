FacefullEventTable = [];

const { BrowserWindow, ipcMain } = require('electron');

ipcMain.on('facefull-bridge-send', (event, command) => {
    const window = BrowserWindow.fromWebContents(event.sender);
    if (typeof FacefullEventTable[command.id] !== 'undefined') {
        try {
            FacefullEventTable[command.id].handler(window, command.data);
        } catch (err) {
            console.error(err.stack);
        }
    }
});

function FacefullBridgeEventAttach(comm, handler = function(sender = null, data = ""){}) {
    FacefullEventTable[comm] = {handler: handler};
}

function FacefullBridgeEventSend(window, command, data) {
    window.webContents.send('facefull-bridge-receive', {command: command, data: data});
}

module.exports = {
    FacefullBridgeEventSend,
    FacefullBridgeEventAttach
};

// insert this to preload.js
// const { contextBridge, ipcRenderer } = require('electron');
//
// contextBridge.exposeInMainWorld('facefull_bridge', {
//     send: (command, data) => {
//         ipcRenderer.send('facefull-bridge-send', {id: command, data: data})
//     },
//     receive: (callback) => ipcRenderer.on('facefull-bridge-receive', (_event, value) => callback(value))
// });
