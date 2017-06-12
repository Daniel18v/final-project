<?php

defined("BASEPATH") or exit("No direct script access allowed");


class User_controller extends MY_Controller {

    /**
     * Inherits of MY_Controller and charges the model for this controller.
     */
    public function __construct() {
        parent::__construct();
        $this->load->model('user_model');
        $this->load->helper('sess');
        $this->load->library('session', 'form');
    }

    /**
     * Allows sign up in webpage
     */
    public function signup() {

        $input_data = array('name'   => $this->input->post('name'),
                            'user'   => $this->input->post('user'),
                            'pass'   => md5($this->input->post('pass')),
                            'mail'   => $this->input->post('mail'),
                            'avatar' => "images/avatars/no-avatar.png",
        );

        $validate = array('name'   => FILTER_SANITIZE_STRING,
                          'user'   => FILTER_SANITIZE_STRING,
                          'mail'   => FILTER_SANITIZE_EMAIL,
                          'pass'   => FILTER_DEFAULT,
                          'avatar' => FILTER_DEFAULT,

        );

        $data = filter_var_array($input_data, $validate);


        if ($this->input->post('pass') === $this->input->post('pass-verify')) {
            $sign = $this->user_model->signup($data);
            $this->login($sign);
            //$jsondata['message'] = "Logueado con éxito";
        } else {
            $jsondata['message'] = "Las contraseñas no coinciden";
        }

        echo json_encode($jsondata);
    }

    public function login() {
        save_uri($this);
        $data = array('user' => $this->input->post('user'), 'pass' => md5($this->input->post('pass')));
        $query = $this->user_model->login($data);
        if ($query) {
            echo "Logueado con éxito";
        } else {
            echo "Se ha producido un fallo";
        }
        redirect_back($this);

    }

    public function logout() {
        save_uri($this);
        $this->session->sess_destroy();
        redirect_back($this);
    }
}