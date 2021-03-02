#ifndef MOPSA_LOGGER_DEF_H
#define MOPSA_LOGGER_DEF_H

#define ASSERT(EXPR) assert_handler((EXPR), #EXPR, __FILE__, __FUNCTION__, __LINE__);

#define LOG(SENSITIVTY) Logger::get_instance().log(SENSITIVTY)

#endif

