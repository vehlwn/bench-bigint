project(
  'bench-bigint',
  ['cpp'],
  default_options: ['cpp_std=c++17'],
  license: 'ISC',
  meson_version: '>=0.60'
)

boost_dep = dependency('boost', include_type: 'system')

executable(
  'bench-bigint',
  ['main.cpp'],
  dependencies: [boost_dep]
)
