<?php

defined("BASEPATH") or exit("No direct script access allowed");


class User_controller extends MY_Controller {

    /**
     * Inherits of MY_Controller and charges the model for this controller.
     */
    public function __construct() {
        parent::__construct();
        $this->load->model('user_model', 'mod');
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


        if ($this->mod->check_mail() === $this->input->post('mail') OR
            $this->mod->check_user() === $this->input->post('user')
        ) {
            $jsondata['status'] = 200;//"Las contraseñas no coinciden";
            echo json_encode($jsondata);
        } else {
            $jsondata['status'] = 416;//"Las contraseñas no coinciden";
            echo json_encode($jsondata);
        }


        if ($this->input->post('pass') === $this->input->post('pass-verify')) {
            $sign = $this->user_model->signup($data);
            $this->login($sign);
        } else {
            $jsondata['status'] = 418;//"Las contraseñas no coinciden";
            echo json_encode($jsondata);
        }
    }

    public function login() {
        save_uri($this);
        $data = array('user' => $this->input->post('user'), 'pass' => md5($this->input->post('pass')));
        $jsondata['status'] = $this->user_model->login($data);
        echo json_encode($jsondata);

    }

    public function logout() {
        save_uri($this);
        $this->session->sess_destroy();
        redirect_back($this);
    }

    public function add_coins() {
        $this->user_model->add_coins($this->input->post(), $this->session->userdata('user'));
    }
}