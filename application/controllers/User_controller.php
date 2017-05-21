<?php

defined("BASEPATH") or exit("No direct script access allowed");


class User_controller extends MY_Controller {
    public function __construct() {
        parent::__construct();
        $this->load->model('user_model');
        $this->load->library('session');
    }

    public function login() {
        $user = $this->input->post('user');
        $pass = md5($this->input->post('pass'));
        $this->user_model->login($user, $pass);
        redirect(base_url());
    }
    public function logout() {
        $this->session->sess_destroy();
        redirect(base_url());
    }
}