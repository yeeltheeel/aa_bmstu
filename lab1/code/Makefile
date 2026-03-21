main_src = lab.cpp
lib_src = lib.cpp
test_src = unit-testing.cpp

main_targ = main
test_targ = unittest
my_dir = build

# ./target if linux

all: build-stuff
	cd $(my_dir) && ctest 
	cd $(my_dir) && $(test_targ) --gtest_output="json:gtest_report.json"
	cd $(my_dir)/CMakeFiles/$(test_targ).dir && gcovr -r ../../../ --filter "../../../$(lib_src)" --json-pretty --json-summary-pretty --json ../../coverage.json > ../../cov.json
	py merge_reports.py

build-stuff:
	@if not exist "$(my_dir)" (mkdir $(my_dir))
	cmake -S . -B $(my_dir)
	cmake --build $(my_dir)

extra:
	cmake --version
	gcovr --version
	pip install gcovr

clean:
	del /s /q build
