
CC                     = "gcc"
CFLAGS                 = ""
FPGA_BITSTREAM_FILE    = "plbram_256k_dbg.bin"
FPGA_BITSTREAM_GZ_FILE = FPGA_BITSTREAM_FILE + ".gz"
DEVICE_TREE_DIRECTORY  = "plbram_256k"
DEVICE_TREE_FILE       = "plbram_256k_dbg.dts"
UIOMEM_DEVICE_NAME     = "uiomem0"

require 'rake/clean'

desc "Install fpga and devicetrees"
task :install => ["/lib/firmware/#{FPGA_BITSTREAM_FILE}", DEVICE_TREE_FILE] do
  begin
    sh "./dtbo-config --install #{DEVICE_TREE_DIRECTORY} --dts #{DEVICE_TREE_FILE}"
  rescue => e
    print "error raised:"
    p e
    abort
  end
  if (File.exist?("/dev/" + UIOMEM_DEVICE_NAME) == false)
    abort "can not uiomem installed."
  end
  File::chmod(0666, "/dev/" + UIOMEM_DEVICE_NAME)
  File::chmod(0666, "/sys/class/uiomem/" + UIOMEM_DEVICE_NAME + "/sync_mode")
  File::chmod(0666, "/sys/class/uiomem/" + UIOMEM_DEVICE_NAME + "/sync_offset")
  File::chmod(0666, "/sys/class/uiomem/" + UIOMEM_DEVICE_NAME + "/sync_size")
  File::chmod(0666, "/sys/class/uiomem/" + UIOMEM_DEVICE_NAME + "/sync_direction")
  File::chmod(0666, "/sys/class/uiomem/" + UIOMEM_DEVICE_NAME + "/sync_owner")
  File::chmod(0666, "/sys/class/uiomem/" + UIOMEM_DEVICE_NAME + "/sync_for_cpu")
  File::chmod(0666, "/sys/class/uiomem/" + UIOMEM_DEVICE_NAME + "/sync_for_device")
end

desc "Uninstall fpga and devicetrees"
task :uninstall do
  device_file = "/dev/" + UIOMEM_DEVICE_NAME
  if (File.exist?(device_file) == false)
    abort "can not #{device_file} uninstalled: does not already exists."
  end
  sh "./dtbo-config --remove #{DEVICE_TREE_DIRECTORY}"
end

file "/lib/firmware/" + FPGA_BITSTREAM_FILE => [ FPGA_BITSTREAM_GZ_FILE ] do
  sh "gzip -d -f -c #{FPGA_BITSTREAM_GZ_FILE} > /lib/firmware/#{FPGA_BITSTREAM_FILE}"
end
CLOBBER.include("/lib/firmware/" + FPGA_BITSTREAM_FILE)

file "/dev/#{UIOMEM_DEVICE_NAME}" do
  Rake::Task["install"].invoke
end

task :default => ["/dev/#{UIOMEM_DEVICE_NAME}"]
