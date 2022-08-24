#!/bin/bash
cd "$(dirname "$0")"

echo "removing old build..."

rm -r build/ dist/ frontend/build/ frontend/dist/

echo "old build removed"
echo "building backend..."

mkdir build
cd build
cmake -Dlog_debug=OFF ..
cmake --build . 

echo "backend builded"
echo "building frontend..."

cd ..
cd frontend
pip install virtualenv
virtualenv venv
source venv/bin/activate
pip install -r requirements.txt
pip install pyinstaller
python -m PyInstaller --onefile --add-data static:static --add-data templates:templates -n gui __main__.py

echo "frontend builded"
echo "putting binaries together..."

cd ..
mkdir dist
cp frontend/dist/gui dist/
cp build/ruba-mazzetto dist/

echo "done"
echo "launching ruba mazzetto..."

cd dist
./ruba-mazzetto