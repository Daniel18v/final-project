<?php
defined("BASEPATH") or exit("No direct script access allowed");

class Login {
    private $ci;

    public function __construct() {
        $this->ci =& get_instance();
        ! $this->ci->load->helper('url') ? $this->ci->load->helper('url') : false;
        $this->ci->load->model('user_model');
    }

    public function check_login() {

//        echo "<pre>". print_r(,true) . "</pre>";
//        if ($this->ci->user_model->check_user($this->ci->session->userdata('id'))) {
//            $this->check_flag();
//        } else {
//            $this->ci->session->set_userdata('rol', '0');
//            base_url();
//            $this->check_flag();
//
//        }
    }
    public function check_flag() {
        echo "<pre>rol del usuario:" . print_r($this->ci->session->userdata('rol'), true)."</pre>";
        echo "<pre>página actual:" . print_r($_SERVER['REQUEST_URI'], true) . "</pre>";
        foreach ($this->ci->config->config['security'] as $key => $v) {

            for ($i = 0; $i < count($v['rol']) ; $i++) {
                echo "<pre>rol de la página:". print_r($v['rol'][$i], true)."</pre>";
                echo "<pre>página comprobada:". print_r($v['url'], true)."</pre>";
//                echo "<pre>".print_r($this->ci->session->userdata('rol')[0], true)."</pre>";
                if ($v['url'] == $_SERVER['REQUEST_URI'] && $v['rol'][$i] == $this->ci->session->userdata('rol')) {


//                    echo "oleoleole" . "<br/>";

                } else if ($this->ci->session->userdata('rol') == 0) {
//                    show_404();
//                        echo "<pre>".print_r($this->ci->session->userdata('rol')[$i], true)."</pre>";
//                        echo $v['rol'][$i] . $this->ci->session->userdata('rol');

//                    echo "no funciono" . "<br/>";

                }
            }
        }
    }
}


//        $this->ci->config->item('emulador')['url'];





