# RUBA MAZZETTO

## Guidelines
[Italian [pdf]](docs/traccia.pdf)
## Documentation

[Doxygen documentation [English]](https://mauroderosa.github.io/ruba-mazzetto/files.html)

[Project overview [Italian][pdf]](docs/overview/overview.pdf)

## Dependencies

### Ubuntu/Debian
```bash
sudo apt install python3-gi python3-gi-cairo gir1.2-gtk-3.0
```
### Fedora
```bash
sudo dnf install python3-gobject gtk3
```

### Arch Linux
```bash
sudo pacman -S python-gobject gtk3
```

### openSUSE
```bash
sudo zypper install python3-gobject python3-gobject-Gdk typelib-1_0-Gtk-3_0 libgtk-3-0
```

## Build instructions
To build this project you need to compile backend and frontend separately.
You will need [Cmake](https://cmake.org/) for your distribution.

### Backend
From the project root
```bash
mkdir build
cd build
cmake -Dlog_debug=OFF ..
cmake --build . 
```
You will obtain an executable with the name **ruba-mazzetto**.

Now let's buld the frontend.
### Frontend
 First be sure to have installed the **dependencies** listed above.
 Then from the project root:
```bash
cd frontend
pip install virtualenv
virtualenv venv
source venv/bin/activate
pip install -r requirements.txt
pip install pyinstaller
python -m PyInstaller --onefile --add-data static:static --add-data templates:templates -n gui __main__.py
```
### Now let's put all together
Now that you have built frontend and backend come back to the project root and put the two executable in a new folder and run ruba-mazzetto.
```bash
mkdir dist
cp frontend/dist/gui dist/
cp build/ruba-mazzetto dist/
cd dist
./ruba-mazzetto
```

## External resources

- [PyWebView](https://pywebview.flowrl.com/) Build GUI for your Python program with JavaScript, HTML, and CSS 
- [jQuery](https://jquery.com/) JavaScript library for DOM manipulation and ajax
- [SimpleGrid](https://simplegrid.io/) a CSS grid for your website. Responsive, light, simple.
- [ChartJs](https://www.chartjs.org/) Simple yet flexible JavaScript charting for designers & developers
- [FontAwesome](https://fontawesome.com/) Font Awesome is the Internet's icon library and toolkit, used by millions of designers, developers, and content creators.
- [DoxygenAwesome](https://github.com/jothepro/doxygen-awesome-css) modern theme for doxygen