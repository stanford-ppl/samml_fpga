set SynModuleInfo {
  {SRCNAME unary MODELNAME unary RTLNAME unary IS_TOP 1
    SUBMODULES {
      {MODELNAME unary_control_s_axi RTLNAME unary_control_s_axi BINDTYPE interface TYPE interface_s_axilite}
      {MODELNAME unary_regslice_both RTLNAME unary_regslice_both BINDTYPE interface TYPE adapter IMPL reg_slice}
      {MODELNAME unary_flow_control_loop_pipe RTLNAME unary_flow_control_loop_pipe BINDTYPE interface TYPE internal_upc_flow_control INSTNAME unary_flow_control_loop_pipe_U}
    }
  }
}
