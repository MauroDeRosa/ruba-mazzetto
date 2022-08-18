let promises = [];
let route_promise = null;

function route(path) {
    return pywebview.api.route(path);
}

function target(path) {
    return pywebview.api.target(path);
}

function varset(name, value) {
    return pywebview.api.varset(name, value);
}

function varget(name) {
    return pywebview.api.varget(name);
}

function backend(message) {
    return pywebview.api.backend(message);
}

function destroy() {
    pywebview.api.destroy();
}