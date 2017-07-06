
static struct resource my_test_resources[] = {
	[0] = {
		.name = "irq_gpio",
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device my_test = {
	.name = "my-test",
	.id = -1,
	.resource = my_test_resources,
	.num_resources = ARRAY_SIZE(my_test_resources),
};


static int init_my_test()
{
	my_test_resources[0].start = gpio_to_irq(TEST_GPIO);
	platform_device_register(&my_test);
}

