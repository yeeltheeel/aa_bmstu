ifeq ($(OS), Windows_NT)
	rem = del /s /q
	newdir = mkdir
	ifdir = @if not exist "$(my_dir)" ($(newdir) $(my_dir))
	copyfile = copy
	movefile = move /y
	renamefile = rename /y
	runexec = 
	ghost = gswin64c
	filesize_st = @for %%F in (
	filesize_fn = ) do @echo    %%~zF bytes
else
	rem = rm -rf
	newdir = mkdir -p
	ifdir = @if [ ! -d "$(my_dir)" ]; then $(newdir) $(my_dir); fi
	copyfile = cp
	movefile = mv -f
	renamefile = mv -f
	runexec = ./
	ghost = gs
	filesize_st = @echo    $$(stat -c%s
	filesize_fn = bytes)
endif

main_src = lab6.cpp
lib_src = sales.cpp
test_src = unit-testing.cpp

main_targ = main
test_targ = unittest
my_dir = build

all: build-stuff
	cd $(my_dir) && ctest 
	cd $(my_dir) && $(runexec)$(test_targ) --gtest_output="json:gtest_report.json"
	cd $(my_dir)/CMakeFiles/$(test_targ).dir && gcovr -r ../../../ --filter "../../../$(lib_src)" --json-pretty --json-summary-pretty --json ../../coverage.json > ../../cov.json
	python3 merge_reports.py
	cd .. && $(newdir) ready
	$(copyfile) stud-unit-test-report.json ../ready

build-stuff:
	$(ifdir)
	cmake -S . -B $(my_dir)
	cmake --build $(my_dir)

clean:
	rm -rf build

