set SynModuleInfo {
  {SRCNAME mem_read MODELNAME mem_read RTLNAME mem_read IS_TOP 1
    SUBMODULES {
      {MODELNAME mem_read_gmem_m_axi RTLNAME mem_read_gmem_m_axi BINDTYPE interface TYPE adapter IMPL m_axi}
      {MODELNAME mem_read_control_s_axi RTLNAME mem_read_control_s_axi BINDTYPE interface TYPE interface_s_axilite}
      {MODELNAME mem_read_regslice_both RTLNAME mem_read_regslice_both BINDTYPE interface TYPE adapter IMPL reg_slice}
      {MODELNAME mem_read_flow_control_loop_pipe RTLNAME mem_read_flow_control_loop_pipe BINDTYPE interface TYPE internal_upc_flow_control INSTNAME mem_read_flow_control_loop_pipe_U}
    }
  }
}
