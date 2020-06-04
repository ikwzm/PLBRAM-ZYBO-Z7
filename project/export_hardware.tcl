#
# export_hardware.tcl  Tcl script for export hardware
#
#
# Open Project
#
set project_directory       [file dirname [info script]]
set project_name            "project"
#
#
#
if {[info exists project_name     ] == 0} {
    set project_name        "project"
}
if {[info exists project_directory] == 0} {
    set project_directory   [pwd]
}
open_project [file join $project_directory $project_name]
#
# Generate Hardware File
#
set design_top_name [get_property "top" [current_fileset]]
write_hw_platform -fixed -force -file [file join $project_directory $design_top_name.xsa]
#
# Close Project
#
close_project
