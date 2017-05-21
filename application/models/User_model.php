<?php

class User_model extends CI_Model {

    function __construct() {
        $this->load->database();
    }

    function login($user, $pass) {
        $this->db->where('user', $user)->from('users');
        $query = $this->db->get();

        if ($query->num_rows() > 0) {
            $this->db->where('pass', $pass)->where('user', $user)->from('users');
            $query = $this->db->get();
            if ($query->num_rows() > 0) {
                $query = $query->row();
                $data  = array('user' => $query->user, 'avatar' => $query->avatar);
                $this->session->set_userdata($data);
            } else {
                $this->session->set_flashdata('error', 'El usuario no es correcto');
            }
        } else {
            $this->session->set_flashdata('error', 'El usuario no es correcto');
        }
    }
}