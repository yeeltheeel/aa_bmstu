main_src = lab2.cpp
lib_src = recursive.cpp
test_src = unit-testing.cpp

main_targ = main
test_targ = unittest
my_dir = build

all: build-stuff
	cd $(my_dir) && ctest 
	cd $(my_dir) && $(test_targ) --gtest_output="json:gtest_report.json"
	cd $(my_dir)/CMakeFiles/$(test_targ).dir && gcovr -r ../../../ --filter "../../../$(lib_src)" --json-pretty --json-summary-pretty --json ../../coverage.json > ../../cov.json
	py merge_reports.py
	mv stud-unit-test-report.json ../ready

build-stuff:
	mkdir -p $(my_dir)
	cmake -S . -B $(my_dir)
	cmake --build $(my_dir)

clean:
	$(rem) build
