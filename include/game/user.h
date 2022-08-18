#ifndef __USER_H__
#define __USER_H__

/**
 * @file user.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief basic user system
 * @version 0.1
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <time.h>
#include <stdbool.h>

#define USERNAME_SIZE 26 ///< username string size
#define USERNAME_MIN_LEN 5 ///< username minimum length
#define USERNAME_MAX_LEN (USERNAME_SIZE - 1) ///< username maximum length
#define PASSWORD_SIZE 26 ///< password string size
#define PASSWORD_MIN_LEN 8 ///< minimum password length
#define PASSWORD_MAX_LEN (PASSWORD_SIZE - 1) ///< maximum password length
#define USERID_INVALID 0 ///< invalid user id

typedef size_t userid; ///< user id

typedef struct
{
    userid id; ///< id of the user
    char username[USERNAME_SIZE]; ///< the username of the user of length
    char password[PASSWORD_SIZE]; ///< plain text user password @todo TODO: add hashing functions
    struct tm created; ///< timestamp of the user creation
    struct tm modified; ///< timestamp fo the last user update
} user; ///< user data structure

/**
 * @brief compare userids
 * 
 * @param a first userid
 * @param b second userid
 * @return int userid difference
 */
int compare_userid(const void* a, const void* b);

/**
 * @brief check if user is logged
 * 
 * @param id id of the user
 * @return true user is logged in
 * @return false user is not logged in
 */
bool is_user_logged(userid id);

/**
 * @brief check if user id is not invalid and is in table
 * 
 * @param id the id of the user to check
 * @return true valid user
 * @return false user not valid
 */
bool is_user_valid(userid id);

/**
 * @brief check the existence of a certain user
 * 
 * @param id the user id you want to check
 * @return true user found
 * @return false user not found
 */
bool user_exists(userid id);

/**
 * @brief check the existence of a certain user
 * 
 * @param username the username you want to check
 * @return true user found
 * @return false user not found
 */
bool username_exists(const char *username);

/**
 * @brief check if the username is between minimum and maximum length,
 * check also if the username is made only by alphanumeric characters and '_'
 * 
 * @param username the username you want to check
 * @return true username meets the required characteristics
 * @return false invalid username
 */
bool is_username_valid(const char *username);

/**
 * @brief check if the password is between minimum and maximum length,
 * check also if the password is made only by alphanumeric and punctuation characters
 * 
 * @param password the password you want to check 
 * @return true the password meets the required characteristics
 * @return false invalid password
 */
bool is_password_valid(const char *password);

/**
 * @brief find a user
 * 
 * @param id the id of the user to get
 * @return user* the found user's pointer
 * @return NULL user not found
 */
user* get_user(userid id);

/**
 * @brief find a user
 * 
 * @param username the username of the user to get
 * @return user* the found user's pointer
 * @return NULL user not found
 */
user* get_user_by_username(const char* username);

/**
 * @brief register a new user
 * 
 * @param username the username of the new user it's checked by is_username_valid @see is_username_valid
 * @param password the password of the new user it's checked by is_password_valid @see is_password_valid
 * @return userid the user has been created successfully @see userid
 * @return USERID_INVALID the user was not created @see USERID_INVALID
 * @todo check for unused userid if maximum id is used
 */
userid user_register(const char *username, const char *password);

/**
 * @brief update user password
 * 
 * @param id the id of the user you want to edit
 * @param password the new password for the user
 * @return userid the user has been updated successfully @see userid
 * @return USERID_INVALID the user wasn't updated @see USERID_INVALID
 */
userid user_update(userid id, const char *password);

/**
 * @brief login with a registered user
 * 
 * @param username the user's username
 * @param password the user's password
 * @return userid id of the logged user @see userid
 * @return USERID_INVALID wrong username or password @see USERID_INVALID
 */
userid user_login(const char *username, const char *password);

/**
 * @brief log out with user
 * 
 * @param id user id
 */
void user_logout(userid id);

/**
 * @brief delete a registered user
 * 
 * @param id the user id
 * @todo decrease user table size if there's too much memory allocated
 */
void user_delete(userid id);

/**
 * @brief initialize the user table and load the user table binary file if exist
 */
void users_init();

/**
 * @brief Get the users count
 */
size_t users_count();

/**
 * @brief save the user table to the user table binary file
 */
void users_save();

/**
 * @brief load the user table from the user table binary file
 */
void user_load();

/**
 * @brief json representation of a user
 * 
 * @param u user pointer
 * @return char* json string (must be freed)
 */
char* user_json(user *u);

void users_free();

#endif /* __USER_H__ */
