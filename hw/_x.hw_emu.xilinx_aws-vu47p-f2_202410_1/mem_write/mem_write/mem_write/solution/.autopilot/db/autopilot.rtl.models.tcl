set SynModuleInfo {
  {SRCNAME mem_write MODELNAME mem_write RTLNAME mem_write IS_TOP 1
    SUBMODULES {
      {MODELNAME mem_write_gmem_m_axi RTLNAME mem_write_gmem_m_axi BINDTYPE interface TYPE adapter IMPL m_axi}
      {MODELNAME mem_write_control_s_axi RTLNAME mem_write_control_s_axi BINDTYPE interface TYPE interface_s_axilite}
      {MODELNAME mem_write_regslice_both RTLNAME mem_write_regslice_both BINDTYPE interface TYPE adapter IMPL reg_slice}
      {MODELNAME mem_write_flow_control_loop_pipe RTLNAME mem_write_flow_control_loop_pipe BINDTYPE interface TYPE internal_upc_flow_control INSTNAME mem_write_flow_control_loop_pipe_U}
    }
  }
}
