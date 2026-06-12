project('journal', 'cpp')

journal_dependencies = []

boost = dependency('boost', version: '>=1.49', modules: ['filesystem', 'system'])
physfs = dependency('physfs', version: '>=2.1')

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Core', 'Widgets', 'Gui'])


journal_sources = [
    'journal.cpp',
    'main.cpp',
    'niko.cpp',
    'pixmap.cpp',
    'pipe.cpp',
]
journal_headers = [
    'journal.h',
    'niko.h',
    'pixmap.h',
    'pipe.h',
]
journal_moc_files = qt5.compile_moc(
    headers: journal_headers
)

journal_dependencies += [
    boost,
    physfs,
    qt5_dep
]

executable(
    '_______',
    journal_sources, journal_moc_files,
    dependencies: journal_dependencies,
    cpp_args: '-std=c++11',
    install: true
)