function(kaldifeat_add_py_test source)
  get_filename_component(name ${source} NAME_WE)
  set(name "${name}_py")

  add_test(NAME ${name}
    COMMAND
      "${PYTHON_EXECUTABLE}"
      "${CMAKE_CURRENT_SOURCE_DIR}/${source}"
  )

  get_filename_component(kaldifeat_path ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

  set_property(TEST ${name}
    PROPERTY ENVIRONMENT "PYTHONPATH=${kaldifeat_path}:$<TARGET_FILE_DIR:_kaldifeat>:$ENV{PYTHONPATH}"
  )
endfunction()

# please sort the files in alphabetic order
set(py_test_files
  test_fbank.py
  test_fbank_options.py
  test_frame_extraction_options.py
  test_mel_bank_options.py
  test_mfcc.py
  test_mfcc_options.py
  test_plp.py
  test_plp_options.py
  test_spectrogram.py
  test_spectrogram_options.py
)

foreach(source IN LISTS py_test_files)
  kaldifeat_add_py_test(${source})
endforeach()
