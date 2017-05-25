<?php
defined("BASEPATH") or exit("No direct script access allowed");

class Login {
    private $ci;

    public function __construct() {
        $this->ci =& get_instance();
        !$this->ci->load->helper('url') ? $this->ci->load->helper('url') : FALSE;

    }

    public function check_login() {

//        echo "<pre>" . print_r($this->ci->session->userdata('user'), true)  . "</pre>";
//        if (!$this->ci->session->userdata('user')) {
//            redirect('/emulador');
//		} else {
//            redirect(base_url());
//        }
    }
}

