test(
  'TestMixRamp',
  executable(
    'TestMixRamp',
    'TestMixRampParser.cxx',
    'TestMixRampInterpolate.cxx',
    include_directories: inc,
    dependencies: [
      log_dep,
      tag_dep,
      util_dep,
      gtest_dep,
    ],
  ),
  protocol: 'gtest',
)

test(
  'TestParseName',
  executable(
    'TestParseName',
    'TestParseName.cxx',
    include_directories: inc,
    dependencies: [
      tag_dep,
      gtest_dep,
    ],
  ),
  protocol: 'gtest',
)
