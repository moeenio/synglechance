journal_dependencies = []

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Core', 'Widgets', 'Gui'])


journal_sources = [
    'journal.cpp',
    'pixmap.cpp',
    'pipe.cpp',
    'niko.cpp'
]
journal_headers = [
    'journal.h',
    'pixmap.h',
    'pipe.h',
    'niko.h'
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
    cpp_args: '-std=c++11'
)