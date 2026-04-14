#ifdef ESP_PLATFORM
#include "esp_littlefs.h"
#else

#include <cstddef>
#include <cstdint>

#include "../esp_error/esp_error_if.hpp"

inline constexpr std::size_t kEspPartitionLabelMaxLength = 17U;

typedef enum : std::uint8_t {
    ESP_PARTITION_TYPE_APP = 0x00,              //!< Application partition type
    ESP_PARTITION_TYPE_DATA = 0x01,             //!< Data partition type
    ESP_PARTITION_TYPE_BOOTLOADER = 0x02,       //!< Bootloader partition type
    ESP_PARTITION_TYPE_PARTITION_TABLE = 0x03,  //!< Partition table type

    ESP_PARTITION_TYPE_ANY = 0xff,  //!< Used to search for partitions with any type
} esp_partition_type_t;

typedef enum : std::uint8_t {
    ESP_PARTITION_SUBTYPE_BOOTLOADER_PRIMARY = 0x00,  //!< Primary Bootloader
    ESP_PARTITION_SUBTYPE_BOOTLOADER_OTA = 0x01,  //!< Temporary OTA storage for Bootloader, where
                                                  //!< the OTA uploads a new Bootloader image

    ESP_PARTITION_SUBTYPE_PARTITION_TABLE_PRIMARY = 0x00,  //!< Primary Partition table
    ESP_PARTITION_SUBTYPE_PARTITION_TABLE_OTA =
        0x01,  //!< Temporary OTA storage for Partition table, where the OTA uploads a new Partition
               //!< table image

    ESP_PARTITION_SUBTYPE_APP_FACTORY = 0x00,  //!< Factory application partition
    ESP_PARTITION_SUBTYPE_APP_OTA_MIN = 0x10,  //!< Base for OTA partition subtypes
    ESP_PARTITION_SUBTYPE_APP_OTA_0 = ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 0,  //!< OTA partition 0
    ESP_PARTITION_SUBTYPE_APP_OTA_1 = ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 1,  //!< OTA partition 1
    ESP_PARTITION_SUBTYPE_APP_OTA_2 = ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 2,  //!< OTA partition 2
    ESP_PARTITION_SUBTYPE_APP_OTA_3 = ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 3,  //!< OTA partition 3
    ESP_PARTITION_SUBTYPE_APP_OTA_4 = ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 4,  //!< OTA partition 4
    ESP_PARTITION_SUBTYPE_APP_OTA_5 = ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 5,  //!< OTA partition 5
    ESP_PARTITION_SUBTYPE_APP_OTA_6 = ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 6,  //!< OTA partition 6
    ESP_PARTITION_SUBTYPE_APP_OTA_7 = ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 7,  //!< OTA partition 7
    ESP_PARTITION_SUBTYPE_APP_OTA_8 = ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 8,  //!< OTA partition 8
    ESP_PARTITION_SUBTYPE_APP_OTA_9 = ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 9,  //!< OTA partition 9
    ESP_PARTITION_SUBTYPE_APP_OTA_10 =
        ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 10,  //!< OTA partition 10
    ESP_PARTITION_SUBTYPE_APP_OTA_11 =
        ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 11,  //!< OTA partition 11
    ESP_PARTITION_SUBTYPE_APP_OTA_12 =
        ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 12,  //!< OTA partition 12
    ESP_PARTITION_SUBTYPE_APP_OTA_13 =
        ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 13,  //!< OTA partition 13
    ESP_PARTITION_SUBTYPE_APP_OTA_14 =
        ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 14,  //!< OTA partition 14
    ESP_PARTITION_SUBTYPE_APP_OTA_15 =
        ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 15,  //!< OTA partition 15
    ESP_PARTITION_SUBTYPE_APP_OTA_MAX =
        ESP_PARTITION_SUBTYPE_APP_OTA_MIN + 16,  //!< Max subtype of OTA partition
    ESP_PARTITION_SUBTYPE_APP_TEST = 0x20,       //!< Test application partition

    ESP_PARTITION_SUBTYPE_DATA_OTA = 0x00,        //!< OTA selection partition
    ESP_PARTITION_SUBTYPE_DATA_PHY = 0x01,        //!< PHY init data partition
    ESP_PARTITION_SUBTYPE_DATA_NVS = 0x02,        //!< NVS partition
    ESP_PARTITION_SUBTYPE_DATA_COREDUMP = 0x03,   //!< COREDUMP partition
    ESP_PARTITION_SUBTYPE_DATA_NVS_KEYS = 0x04,   //!< Partition for NVS keys
    ESP_PARTITION_SUBTYPE_DATA_EFUSE_EM = 0x05,   //!< Partition for emulate eFuse bits
    ESP_PARTITION_SUBTYPE_DATA_UNDEFINED = 0x06,  //!< Undefined (or unspecified) data partition

    ESP_PARTITION_SUBTYPE_DATA_ESPHTTPD = 0x80,  //!< ESPHTTPD partition
    ESP_PARTITION_SUBTYPE_DATA_FAT = 0x81,       //!< FAT partition
    ESP_PARTITION_SUBTYPE_DATA_SPIFFS = 0x82,    //!< SPIFFS partition
    ESP_PARTITION_SUBTYPE_DATA_LITTLEFS = 0x83,  //!< LITTLEFS partition

#if __has_include("extra_partition_subtypes.inc")
#include "extra_partition_subtypes.inc"
#endif

    ESP_PARTITION_SUBTYPE_ANY = 0xff,  //!< Used to search for partitions with any subtype
} esp_partition_subtype_t;

typedef struct {
    const struct spi_flash_host_driver_s *driver;  ///< Pointer to the implementation function table
    // Implementations can wrap this structure into their own ones, and append other data here
} spi_flash_host_inst_t;

typedef struct {
    uint32_t idle_timeout;  ///< Default timeout for other commands to be sent by host and get done
                            ///< by flash
    uint32_t chip_erase_timeout;    ///< Timeout for chip erase operation
    uint32_t block_erase_timeout;   ///< Timeout for block erase operation
    uint32_t sector_erase_timeout;  ///< Timeout for sector erase operation
    uint32_t page_program_timeout;  ///< Timeout for page program operation
} flash_chip_op_timeout_t;

struct esp_flash_t;  // Forward declaration

typedef struct {
    uint32_t offset;  ///< Start address of this region
    uint32_t size;    ///< Size of the region
} esp_flash_region_t;

typedef enum : std::uint8_t {
    SPI_FLASH_SLOWRD = 0,  ///< Data read using single I/O, some limits on speed
    SPI_FLASH_FASTRD,      ///< Data read using single I/O, no limit on speed
    SPI_FLASH_DOUT,        ///< Data read using dual I/O
    SPI_FLASH_DIO,         ///< Both address & data transferred using dual I/O
    SPI_FLASH_QOUT,        ///< Data read using quad I/O
    SPI_FLASH_QIO,         ///< Both address & data transferred using quad I/O
#define SPI_FLASH_OPI_FLAG \
    16  ///< A flag for flash work in opi mode, the io mode below are opi, above are SPI/QSPI mode.
        ///< DO NOT use this value in any API.
    SPI_FLASH_OPI_STR =
        SPI_FLASH_OPI_FLAG,   ///< Only support on OPI flash, flash read and write under STR mode
    SPI_FLASH_OPI_DTR,        ///< Only support on OPI flash, flash read and write under DTR mode
    SPI_FLASH_READ_MODE_MAX,  ///< The fastest io mode supported by the host is
                              ///< ``ESP_FLASH_READ_MODE_MAX-1``.
} esp_flash_io_mode_t;

typedef enum : std::uint8_t {
    SPI_FLASH_REG_STATUS = 1,
} spi_flash_register_t;

#define BIT(n) (1UL << (n))

typedef enum : std::uint8_t {
    SPI_FLASH_CHIP_CAP_SUSPEND = BIT(0),  ///< Flash chip supports suspend/resume during write/erase
    SPI_FLASH_CHIP_CAP_32MB_SUPPORT = BIT(1),  ///< Flash chip driver supports flash size > 32MB
    SPI_FLASH_CHIP_CAP_UNIQUE_ID = BIT(2),     ///< Flash chip supports reading a unique 64-bit ID
} spi_flash_caps_t;

extern esp_err_t (*sus_setup)(esp_flash_t *chip);

struct spi_flash_chip_t {
    const char *name;                        ///< Name of the chip driver
    const flash_chip_op_timeout_t *timeout;  ///< Timeout configuration for this chip
    esp_err_t (*probe)(struct esp_flash_t *chip, uint32_t flash_id);
    esp_err_t (*reset)(struct esp_flash_t *chip);
    esp_err_t (*detect_size)(struct esp_flash_t *chip, uint32_t *size);
    esp_err_t (*erase_chip)(struct esp_flash_t *chip);
    esp_err_t (*erase_sector)(struct esp_flash_t *chip, uint32_t sector_address);
    esp_err_t (*erase_block)(struct esp_flash_t *chip, uint32_t block_address);
    uint32_t sector_size; /* Sector is minimum erase size */
    uint32_t
        block_erase_size; /* Optimal (fastest) block size for multi-sector erases on this chip */
    esp_err_t (*get_chip_write_protect)(struct esp_flash_t *chip, bool *out_write_protected);
    esp_err_t (*set_chip_write_protect)(struct esp_flash_t *chip, bool chip_write_protect);
    uint8_t num_protectable_regions;
    const esp_flash_region_t *protectable_regions;
    esp_err_t (*get_protected_regions)(struct esp_flash_t *chip, uint64_t *regions);
    esp_err_t (*set_protected_regions)(struct esp_flash_t *chip, uint64_t regions);
    esp_err_t (*read)(struct esp_flash_t *chip, void *buffer, uint32_t address, uint32_t length);
    esp_err_t (*write)(struct esp_flash_t *chip, const void *buffer, uint32_t address,
                       uint32_t length);
    esp_err_t (*program_page)(struct esp_flash_t *chip, const void *buffer, uint32_t address,
                              uint32_t length);
    uint32_t page_size;
    esp_err_t (*write_encrypted)(struct esp_flash_t *chip, const void *buffer, uint32_t address,
                                 uint32_t length);
    esp_err_t (*wait_idle)(struct esp_flash_t *chip, uint32_t timeout_us);
    esp_err_t (*set_io_mode)(struct esp_flash_t *chip);
    esp_err_t (*get_io_mode)(struct esp_flash_t *chip, esp_flash_io_mode_t *out_io_mode);
    esp_err_t (*read_id)(struct esp_flash_t *chip, uint32_t *out_chip_id);
    esp_err_t (*read_reg)(struct esp_flash_t *chip, spi_flash_register_t reg_id, uint32_t *out_reg);
    esp_err_t (*yield)(struct esp_flash_t *chip, uint32_t wip);
    esp_err_t (*sus_setup)(struct esp_flash_t *chip);
    esp_err_t (*read_unique_id)(struct esp_flash_t *chip, uint64_t *flash_unique_id);
    spi_flash_caps_t (*get_chip_caps)(struct esp_flash_t *chip);
    esp_err_t (*config_host_io_mode)(struct esp_flash_t *chip, uint32_t flags);
};

typedef struct {
    esp_err_t (*start)(void *arg);
    esp_err_t (*end)(void *arg);
    esp_err_t (*region_protected)(void *arg, size_t start_addr, size_t size);
    esp_err_t (*delay_us)(void *arg, uint32_t microseconds);
    void *(*get_temp_buffer)(void *arg, size_t reqest_size, size_t *out_size);
    void (*release_temp_buffer)(void *arg, void *temp_buf);

#define SPI_FLASH_YIELD_REQ_YIELD BIT(0)
#define SPI_FLASH_YIELD_REQ_SUSPEND BIT(1)

    esp_err_t (*check_yield)(void *arg, uint32_t chip_status, uint32_t *out_request);

#define SPI_FLASH_YIELD_STA_RESUME BIT(2)

    esp_err_t (*yield)(void *arg, uint32_t *out_status);
    int64_t (*get_system_time)(void *arg);

#define SPI_FLASH_OS_IS_ERASING_STATUS_FLAG BIT(0)

    void (*set_flash_op_status)(uint32_t op_status);
} esp_flash_os_functions_t;

struct esp_flash_t {
    spi_flash_host_inst_t *host;  ///< Pointer to hardware-specific "host_driver" structure. Must be
                                  ///< initialized before used.
    const struct spi_flash_chip_t
        *chip_drv;  ///< Pointer to chip-model-specific "adapter" structure. If NULL, will be
                    ///< detected during initialisation.

    const esp_flash_os_functions_t *
        os_func;  ///< Pointer to os-specific hook structure. Call ``esp_flash_init_os_functions()``
                  ///< to setup this field, after the host is properly initialized.
    void *os_func_data;  ///< Pointer to argument for os-specific hooks. Left NULL and will be
                         ///< initialized with ``os_func``.

    esp_flash_io_mode_t
        read_mode;  ///< Configured SPI flash read mode. Set before ``esp_flash_init`` is called.
    uint32_t
        size;  ///< Size of SPI flash in bytes. If 0, size will be detected during initialisation.
               ///< Note: this stands for the size in the binary image header. If you want to get
               ///< the flash physical size, please call `esp_flash_get_physical_size`.
    uint32_t chip_id;   ///< Detected chip id.
    uint32_t busy : 1;  ///< This flag is used to verify chip's status.
    uint32_t
        hpm_dummy_ena : 1;  ///< This flag is used to verify whether flash works under HPM status.
    uint32_t reserved_flags : 30;  ///< reserved.
};

extern esp_err_t (*probe)(esp_flash_t *chip, uint32_t flash_id);

typedef struct {
    uint8_t reserved;          ///< Reserved, must be 0.
    uint8_t mosi_len;          ///< Output data length, in bytes
    uint8_t miso_len;          ///< Input data length, in bytes
    uint8_t address_bitlen;    ///< Length of address in bits, set to 0 if command does not need an
                               ///< address
    uint32_t address;          ///< Address to perform operation on
    const uint8_t *mosi_data;  ///< Output data to slave
    uint8_t *miso_data;        ///< [out] Input data from slave, little endian
    uint32_t flags;            ///< Flags for this transaction. Set to 0 for now.
#define SPI_FLASH_TRANS_FLAG_CMD16 BIT(0)  ///< Send command of 16 bits
#define SPI_FLASH_TRANS_FLAG_IGNORE_BASEIO \
    BIT(1)  ///< Not applying the basic io mode configuration for this transaction
#define SPI_FLASH_TRANS_FLAG_BYTE_SWAP \
    BIT(2)  ///< Used for DTR mode, to swap the bytes of a pair of rising/falling edge
#define SPI_FLASH_TRANS_FLAG_PE_CMD \
    BIT(3)  ///< Indicates that this transaction is to erase/program flash chip.
    uint16_t command;          ///< Command to send
    uint8_t dummy_bitlen;      ///< Basic dummy bits to use
    uint32_t io_mode;          ///< Flash working mode when `SPI_FLASH_IGNORE_BASEIO` is specified.
} spi_flash_trans_t;

typedef struct {
    uint32_t sus_mask;  ///< SUS/SUS1/SUS2 bit in flash register.
    struct {
        uint32_t cmd_rdsr : 8;  ///< Read flash status register(2) command.
        uint32_t sus_cmd : 8;   ///< Flash suspend command.
        uint32_t res_cmd : 8;   ///< Flash resume command.
        uint32_t reserved : 8;  ///< Reserved, set to 0.
    };
} spi_flash_sus_cmd_conf;

struct spi_flash_host_driver_s {
    esp_err_t (*dev_config)(spi_flash_host_inst_t *host);
    esp_err_t (*common_command)(spi_flash_host_inst_t *host, spi_flash_trans_t *transaction);
    esp_err_t (*read_id)(spi_flash_host_inst_t *host, uint32_t *identifier);
    void (*erase_chip)(spi_flash_host_inst_t *host);
    void (*erase_sector)(spi_flash_host_inst_t *host, uint32_t start_address);
    void (*erase_block)(spi_flash_host_inst_t *host, uint32_t start_address);
    esp_err_t (*read_status)(spi_flash_host_inst_t *host, uint8_t *out_sr);
    esp_err_t (*set_write_protect)(spi_flash_host_inst_t *host, bool write_protect_enabled);
    void (*program_page)(spi_flash_host_inst_t *host, const void *buffer, uint32_t address,
                         uint32_t length);
    bool (*supports_direct_write)(spi_flash_host_inst_t *host, const void *buffer_pointer);
    int (*write_data_slicer)(spi_flash_host_inst_t *host, uint32_t address, uint32_t len,
                             uint32_t *align_addr, uint32_t page_size);
    esp_err_t (*read)(spi_flash_host_inst_t *host, void *buffer, uint32_t address,
                      uint32_t read_len);
    bool (*supports_direct_read)(spi_flash_host_inst_t *host, const void *buffer_pointer);
    int (*read_data_slicer)(spi_flash_host_inst_t *host, uint32_t address, uint32_t len,
                            uint32_t *align_addr, uint32_t page_size);
    uint32_t (*host_status)(spi_flash_host_inst_t *host);
    esp_err_t (*configure_host_io_mode)(spi_flash_host_inst_t *host, uint32_t command,
                                        uint32_t addr_bitlen, int dummy_bitlen_base,
                                        esp_flash_io_mode_t io_mode);
    void (*poll_cmd_done)(spi_flash_host_inst_t *host);
    esp_err_t (*flush_cache)(spi_flash_host_inst_t *host, uint32_t addr, uint32_t size);
    void (*check_suspend)(spi_flash_host_inst_t *host);
    void (*resume)(spi_flash_host_inst_t *host);
    void (*suspend)(spi_flash_host_inst_t *host);
    esp_err_t (*sus_setup)(spi_flash_host_inst_t *host, const spi_flash_sus_cmd_conf *sus_conf);
};

typedef struct {
    esp_flash_t *flash_chip;                 /*!< SPI flash chip on which the partition resides */
    esp_partition_type_t type;               /*!< partition type (app/data) */
    esp_partition_subtype_t subtype;         /*!< partition subtype */
    uint32_t address;                        /*!< starting address of the partition in flash */
    uint32_t size;                           /*!< size of the partition, in bytes */
    uint32_t erase_size;                     /*!< size the erase operation should be aligned to */
    char label[kEspPartitionLabelMaxLength]; /*!< partition label, zero-terminated ASCII string */
    bool encrypted;                          /*!< flag is set to true if partition is encrypted */
    bool readonly;                           /*!< flag is set to true if partition is read-only */
} esp_partition_t;

typedef struct {
    const char *base_path;       /**< Mounting point. */
    const char *partition_label; /**< Label of partition to use. If partition_label, partition, and
                                    sdcard are all NULL, then the first partition with data subtype
                                    'littlefs' will be used. */
    const esp_partition_t *partition; /**< partition to use if partition_label is NULL */

#ifdef CONFIG_LITTLEFS_SDMMC_SUPPORT
    sdmmc_card_t *
        sdcard; /**< SD card handle to use if both esp_partition handle & partition label is NULL */
#endif

    uint8_t format_if_mount_failed : 1; /**< Format the file system if it fails to mount. */
    uint8_t read_only : 1;              /**< Mount the partition as read-only. */
    uint8_t dont_mount : 1;             /**< Don't attempt to mount.*/
    uint8_t grow_on_mount : 1;          /**< Grow filesystem to match partition size on mount.*/
} esp_vfs_littlefs_conf_t;

#endif