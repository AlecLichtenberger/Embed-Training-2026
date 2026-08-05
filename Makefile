infantry-build:
	cmake -S mini-repo -B mini-repo/build -G Ninja
	cmake --build mini-repo/build --target infantry_check