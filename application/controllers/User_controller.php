<?php

defined("BASEPATH") or exit("No direct script access allowed");


class User_controller extends MY_Controller {
    public function __construct() {
        parent::__construct();
        $this->load->model('user_model');
        $this->load->library('session', 'form');
    }

    public function signup() {
        $data = array(
                'name' => $this->input->post('name'),
                'user' => $this->input->post('user'),
                'pass' => md5($this->input->post('pass')),
                'mail' => $this->input->post('mail'),
                'avatar' => $this->input->post('userfile')
        );

        $config['upload_path']   = 'images/avatars/';
        $config['allowed_types'] = 'gif|jpg|png';

        $this->load->library('upload', $config);
        $this->upload->initialize($config);
        $file = $data['avatar'];

        echo "<pre>" .  print_r($data, true) . "</pre>";
        echo "<pre>" .  print_r($this->upload->do_upload('userfile'), true) . "</pre>";

        if ($data['avatar'] === NULL) {
            $data['avatar'] = "images/avatars/no-avatar.png";
        } else if ($this->upload->do_upload('userfile')) {
            $data = array('upload_data' => $this->upload->data());
        }
        echo "<pre>" .  print_r($data['upload_data'], true) . "</pre>";



        if ($this->input->post('pass') === $this->input->post('pass-verify')) {
            $sign = $this->user_model->signup($data);
            $this->login($sign);
            echo $data['avatar'];
            //redirect(base_url());
        } else {
            //redirect(base_url());
        }
    }

    public function login() {
        $data = array('user' => $this->input->post('user'), 'pass' => md5($this->input->post('pass')));
        $this->user_model->login($data);
        redirect(base_url());
    }

    public function logout() {
        $this->session->sess_destroy();
        redirect(base_url());
    }
}