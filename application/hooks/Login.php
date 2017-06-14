<?php
defined("BASEPATH") or exit("No direct script access allowed");

class Login extends CI_Hooks {
    private $ci;

    public function __construct() {
        $this->ci =& get_instance();
        !$this->ci->load->helper('url') ? $this->ci->load->helper('url') : FALSE;
        $this->ci->load->model('user_model');
    }

    public function check_login() {
        foreach ($this->ci->config->config['security'] as $key => $value) {
            if (preg_match($value['url'], $this->ci->uri->uri_string())) {
                $this->ci->session->userdata('rol') == NULL ? $this->ci->session->set_userdata('rol', 0) : NULL;
                in_array($this->ci->session->userdata('rol'), $value['rol']) ? : show_403();
            }
        }
    }
}
