#include <game/user.h>
#include <types/array.h>
#include <types/vector.h>
#include <types/memory.h>
#include <io/log.h>
#include <utils/timeutils.h>
#include <utils/mathutils.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#define USER_TABLE_PATH "./users.bin" ///< the user vector binary file path
#define USERID_MAX UINT_MAX           ///< maximum user id

vector *users; ///< the users vector
vector *logged; ///< the logged users vector
size_t last_id;   ///< last used id

int compare_userids(const void *a, const void *b)
{
    const userid *au = a, *bu = b;
    // return user ids numeric difference
    return *au - *bu;
}

bool is_user_logged(userid id)
{
    userid *found = binary_search(&id, logged->elements, logged->count, logged->element_size, compare_userids);
    // if userid is in logged vector return true
    return found != NULL;
}

bool is_user_valid(userid id)
{
    if (id == USERID_INVALID || user_exists(id) == false)
    {
        return false;
    }
    
    // return true if user id is valid and exists in users vector
    return true;
}

int compare_by_username(const void *a, const void *b)
{
    const user *ua = a, *ub = b;

    // return username string comparision 
    return strcmp(ua->username, ub->username);
}

user *get_user_by_username(const char *username)
{
    if (is_username_valid(username) == false)
    {
        log_error("invalid username");
    }

    user value;
    strcpy(value.username, username);
    value.username[USERNAME_MAX_LEN] = '\0';
    // return pointer to the found user or NULL if user wasn't fount
    return binary_search(&value, users->elements, users->count, sizeof(user), compare_by_username);
}

int compare_by_userid(const void *a, const void *b)
{
    const user *ua = a, *ub = b;
    // users id difference
    return (int)(ua->id) - (int)(ub->id);
}

user *get_user(userid id)
{
    user value = (user){.id = id};
    // return user pointer if user with specified id exist else return NULL
    return binary_search(&value, users->elements, users->count, sizeof(user), compare_by_userid);
}

bool user_exists(userid id)
{
    // true if user was found
    return get_user(id) != NULL;
}

bool username_exists(const char *username)
{
    // true if user was found
    return get_user_by_username(username) != NULL;
}

bool is_username_valid(const char *username)
{
    size_t username_len = strlen(username);

    if (username_len < USERNAME_MIN_LEN || username_len > USERNAME_MAX_LEN)
    {
        // if user exceeds max length or is too short return false
        return false;
    }

    for (size_t i = 0; i < username_len; i++)
    {
        // user can contain only alphanumeric characters or underscores
        if (!isalnum(username[i]) && username[i] != '_')
        {
            return false;
        }
    }

    return true;
}

bool is_password_valid(const char *password)
{
    check_null_pointer((void*) password);
    size_t nupper = 0, nlower = 0, ndigit = 0, nspecial = 0;
    char *p = (char*) password, c;
    size_t password_len = strlen(password);

    while (*p != '\0')
    {
        c = *p++;
        if(isupper(c)) nupper++;
        else if(islower(c)) nlower++;
        else if(isdigit(c)) ndigit++;
        else if(ispunct(c)) nspecial++;
        else return false; // if invalid character return not valid
    } 

    // returns true if password contains at least 1 uppercase char, 1 punctuation symbol and 1 digit, and it's of the right length
    return (nupper && nlower && ndigit && nspecial) && (password_len >= PASSWORD_MIN_LEN && password_len <= PASSWORD_MAX_LEN);
}

size_t users_count()
{
    return users->count;
}

userid user_register(const char *username, const char *password)
{
    if (last_id == USERID_MAX)
    {
        log_warning("user limit reached, no more users can be created");
        return USERID_INVALID;
    }

    if (is_username_valid(username) == false)
    {
        log_warning("username must be at least 5 characters long, maximum 25. can contain only letters and numbers and underscore '_'");
        return USERID_INVALID;
    }

    if (is_password_valid(password) == false)
    {
        log_warning("password must contain at least 1 upper character, 1 digit, 1 special character. and must be at least 8 characters long, max 25");
        return USERID_INVALID;
    }

    if (username_exists(username))
    {
        log_warning("user %s already exist", username);
        return USERID_INVALID;
    }

    user new_user;

    // set creation and modified timestamp to current time
    new_user.created = datetime_now();
    new_user.modified = new_user.created;
    // set username and hashed password
    strcpy(new_user.username, username);
    new_user.password = hash((unsigned char*) password);
    // increase last id and set it to the user
    new_user.id = ++last_id;

    // append newly created user to the vector and save users file
    VEC_APPEND(user, users, new_user);
    users_save();

    // return created user id
    return new_user.id;
}

userid user_update(userid id, const char *password)
{
    if (is_password_valid(password) == false)
    {
        log_warning("invalid password");
        return USERID_INVALID;
    }

    user *u = get_user(id);

    if (u == NULL)
    {
        log_warning("user with id %zu doesn't exist", id);
        return USERID_INVALID;
    }

    // update modified timestamp to current time
    u->modified = datetime_now();
    // update hashed password
    u->password = hash((unsigned char*) password);
    // save users file and return updated user id
    users_save();
    return id;
}

userid user_login(const char *username, const char *password)
{
    if (is_username_valid(username) == false)
    {
        log_warning("invalid username");
        return USERID_INVALID;
    }

    if (is_password_valid(password) == false)
    {
        log_warning("invalid password");
        return USERID_INVALID;
    }

    // get user with the given username
    user *u = get_user_by_username(username);

    if (u == NULL)
    {
        // if not found
        log_warning("user %s doesn't exist", username);
        return USERID_INVALID;
    }
    else if (is_user_logged(u->id))
    {
        // if user is already logged in
        log_warning("user %s is already logged in", u->username, u->id);
        return USERID_INVALID;
    }
    else if (u->password == hash((unsigned char*) password))
    {
        // if hashed password matched append to logged vector and return logged user id
        VEC_APPEND(userid, logged, u->id);
        return u->id;
    }
    else
    {
        // if wrong password
        log_warning("wrong password");
        return USERID_INVALID;
    }
}

void user_logout(userid id)
{
    // get logged user with the given id
    userid *found = binary_search(&id, logged->elements, logged->count, logged->element_size, compare_by_userid);
    
    if (found != NULL)
    {
        // if userid exist remove it from the logged vector
        size_t index = array_index_from_pointer(logged->elements, found, logged->element_size);
        VEC_DELETE(logged, index);
    }
    else
    {
        log_warning("%zu id isn't logged in", id);
    }
}

// TODO: unused for now
void user_delete(userid id)
{
    user *u = get_user(id);

    if (u == NULL)
    {
        log_error("user with id %zu doesn't exist", id);
    }

    size_t index = array_index_from_pointer(users->elements, u, sizeof(user));
    VEC_DELETE(users, index);
    users_save();
}


void users_save()
{
    // open users file and write last id, users count and users vector
    FILE *f = fopen(USER_TABLE_PATH, "wb");
    fwrite(&last_id, sizeof(userid), 1, f);
    fwrite(&users->count, sizeof(size_t), 1, f);
    fwrite(users->elements, users->element_size, users->count, f);
    fclose(f);
}

void user_load()
{
    // open users file load last id, users count, resize vector if necessary and load vector data
    FILE *f = fopen(USER_TABLE_PATH, "rb");
    fread(&last_id, sizeof(userid), 1, f);
    fread(&users->count, sizeof(size_t), 1, f);

    if (users->count > users->size)
    {
        VEC_RESIZE(users, users->count);
    }

    fread(users->elements, users->element_size, users->count, f);
    fclose(f);
}

void users_init()
{
    // on program closure free users and logged vectors
    atexit(users_free);

    //initialize vectors
    users = VEC_INIT(user);
    logged = VEC_INIT(userid);

    // file exists
    if (access(USER_TABLE_PATH, F_OK) == 0)
    {
        // load users file
        user_load();
    }
    else
    {
        // set last_id to initial invalid id
        last_id = USERID_INVALID;
    }
}

char *user_json(user *u)
{
    check_null_pointer(u);
    char *json = memory_allocate_zero(400, sizeof(char));
    char created[DATETIME_STR_SIZE] = {0}, modified[DATETIME_STR_SIZE] = {0};
    datetime_string(u->created, created);
    datetime_string(u->modified, modified);
    sprintf(json, "{\"id\": %zu,\"username\": \"%s\",\"created\": \"%s\",\"modified\": \"%s\"}", u->id, u->username, created, modified);
    return json;
}

void users_free() {
    VEC_FREE(users);
    VEC_FREE(logged);
}