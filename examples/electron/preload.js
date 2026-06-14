const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('facefull_bridge', {
    send: (command, data) => {
        ipcRenderer.send('facefull-bridge-send', {id: command, data: data})
    },
    receive: (callback) => ipcRenderer.on('facefull-bridge-receive', (_event, value) => callback(value))
});
