import json

report = 'stud-unit-test-report.json'
gtest_report = 'build/gtest_report.json'
gcovr_report = 'build/cov.json'

gfile = open(gtest_report, 'r')
cfile = open(gcovr_report, 'r')
gdata = json.load(gfile)
cdata = json.load(cfile)
cfile.close()
gfile.close()

data = dict()
data['timestamp'] = gdata['timestamp']
data['coverage'] = cdata['line_percent']
data['passed'] = gdata['tests'] - gdata['failures']
data['failed'] = gdata['failures']

file = open(report, 'w')
json.dump(data, file, indent=4)
file.close()

