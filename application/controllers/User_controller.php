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
        $input_data = array(
            'name' => $this->input->post('name'),
            'user' => $this->input->post('user'),
            'mail' => $this->input->post('mail'),
            'pass' => md5($this->input->post('pass')),
            'avatar' => "/images/avatars/no-avatar.png",
            'bornDate' => $this->input->post('bornDate')
        );
        $validate = array(
            'name' => FILTER_SANITIZE_STRING,
            'user' => FILTER_SANITIZE_STRING,
            'mail' => FILTER_SANITIZE_EMAIL,
            'pass' => FILTER_DEFAULT,
            'avatar' => FILTER_DEFAULT,
            'bornDate' => FILTER_DEFAULT
        );
        $data = filter_var_array($input_data, $validate);
        if ($this->mod->check_user($data['user']) === $data['user']) {
            $jsondata['status'] = 416;
            echo json_encode($jsondata);
        } else if ($this->mod->check_mail($data['mail']) === $data['mail']) {
            $jsondata['status'] = 417;
            echo json_encode($jsondata);
        } elseif ($this->input->post('pass') === $this->input->post('pass-verify')) {
            $this->mod->signup($data);
            $this->login();
        } else {
            $jsondata['status'] = 415;
            echo json_encode($jsondata);
        }
    }
    public function login() {
        save_uri($this);
        $data = array('user' => $this->input->post('user'), 'pass' => md5($this->input->post('pass')));
        $jsondata['status'] = $this->mod->login($data);
        echo json_encode($jsondata);

    }

    public function logout() {
        save_uri($this);
        $this->session->sess_destroy();
        redirect_back($this);
    }

    public function add_coins() {
        echo $this->user_model->add_coins($this->input->post(), $this->session->userdata('user'));
    }

    public function add_coins_comment() {
        $this->user_model->add_coins($data = ['bird_coins' => 2], $this->session->userdata('user'));
    }

    public function buy_game() {
        echo $this->user_model->buy_game($this->input->post(), $this->session->userdata('user'));
    }

}