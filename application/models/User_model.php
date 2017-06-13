<?php

defined("BASEPATH") or exit("No direct script access allowed");


class User_model extends CI_Model {

    function __construct() {
        $this->load->database();
    }

    function login($data) {
        $this->db->where('user', $data['user'])->from('users');
        $query = $this->db->get();

        if ($query->num_rows() > 0) {
            $this->db->where('pass', $data['pass'])->where('user', $data['user'])->from('users');
            $query = $this->db->get();
            if ($query->num_rows() > 0) {
                $query = $query->row();
                $recover_data = array(
                    'user' => $query->user,
                    'avatar' => $query->avatar,
                    'id' => $query->id,
                    'rol' => $query->rol,
                    'bird_coins' => $query->bird_coins
                );
                $this->session->set_userdata($recover_data);
                return 200;
            } else {
                return 420;
            }
        } else {
            return 417;
        }
    }

    function signup($data) {
        $this->db->insert('users', $data);
        return $data;
    }

    function check_user($id) {
        if ($id === NULL) {
            return FALSE;
        }
        $this->db->where('id', $id)->from('users');
        $query = $this->db->get();
        if ($query->num_rows() > 0) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
    function add_coins($data, $user) {
        $this->db->select(array('bird_coins'))
            ->where('user',$user)
            ->get_compiled_select('users', FALSE);
        $coins_user = (int)$this->db->get()->result_array()[0]['bird_coins'];
        $data['bird_coins'] =  $data['bird_coins'] * 10;
        $data['bird_coins'] += $coins_user;
        echo $data['bird_coins'];
        $this->db->update('users', $data,  array('user' => $user));
        $this->session->set_userdata('bird_coins', $data['bird_coins']);
    }
}
